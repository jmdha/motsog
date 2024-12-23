#include <assert.h>
#include <stdbool.h>

#include "bit.h"
#include "masks.h"
#include "move.h"
#include "move_gen.h"
#include "position.h"
#include "utility.h"

Move *BuildPawnMoves(Move *moves, BB targets, int delta, enum MoveType move_type) {
    while (targets) {
        Square dst = lsbpop(&targets);
        *(moves++) = move_make(dst - delta, dst, move_type);
    }
    return moves;
}

Move *BuildPawnPromotionMoves(Move *moves, BB targets, int delta, bool capture) {
    while (targets) {
        Square dst = lsbpop(&targets);
        *(moves++) = move_make(dst - delta, dst, NPromotion + 4 * capture);
        *(moves++) = move_make(dst - delta, dst, BPromotion + 4 * capture);
        *(moves++) = move_make(dst - delta, dst, RPromotion + 4 * capture);
        *(moves++) = move_make(dst - delta, dst, QPromotion + 4 * capture);
    }
    return moves;
}

Move *GeneratePawnCaptures(Move *moves, Color turn, BB pawns, BB empty, BB nus, Square ep) {
    static const BB PROMOTED_RANK = RANK_8_BB | RANK_1_BB;
    const int left_capture_delta = turn == WHITE ? 7 : -9;
    const int right_capture_delta = turn == WHITE ? 9 : -7;

    BB advanced = shift_up(turn, pawns) & empty;
    BB promoted = advanced & PROMOTED_RANK;
    advanced ^= promoted;

    // Remove those capture reached from opposite side of board
    // Do note, this is left/right from the perspective of white
    BB left_captures = nus & (((turn == WHITE) ? (pawns << 7) : (pawns >> 9)) & ~FILE_8_BB);
    BB right_captures = nus & (((turn == WHITE) ? (pawns << 9) : (pawns >> 7)) & ~FILE_1_BB);
    BB left_promotion_captures = left_captures & PROMOTED_RANK;
    BB right_promotion_captures = right_captures & PROMOTED_RANK;
    left_captures ^= left_promotion_captures;
    right_captures ^= right_promotion_captures;

    moves = BuildPawnMoves(moves, left_captures, left_capture_delta, Capture);
    moves = BuildPawnMoves(moves, right_captures, right_capture_delta, Capture);
    moves = BuildPawnPromotionMoves(moves, left_promotion_captures, left_capture_delta, true);
    moves = BuildPawnPromotionMoves(moves, right_promotion_captures, right_capture_delta, true);

    // If no EnPassant is allowed, i.e. a double pawn move was not done last turn
    // then the ep square is square none
    assert(ATTACKS_PAWN[!turn][SQUARE_NONE] == 0);
    BB ep_pawns = ATTACKS_PAWN[!turn][ep] & pawns;
    while (ep_pawns)
        *(moves++) = move_make(lsbpop(&ep_pawns), ep, EPCapture);

    return moves;
}

Move *GeneratePawnQuiet(Move *moves, Color turn, BB pawns, BB empty) {
    static const BB ADVANCED_ONCE[COLOR_COUNT] = {RANK_3_BB, RANK_6_BB};
    static const BB PROMOTED_RANK = RANK_8_BB | RANK_1_BB;
    const int move_delta = turn == WHITE ? 8 : -8;

    BB advanced = shift_up(turn, pawns) & empty;
    BB advanced_double = shift_up(turn, advanced & ADVANCED_ONCE[turn]) & empty;
    BB promoted = advanced & PROMOTED_RANK;
    advanced ^= promoted;

    moves = BuildPawnMoves(moves, advanced, move_delta, Quiet);
    moves = BuildPawnMoves(moves, advanced_double, 2 * move_delta, DoublePawnPush);
    moves = BuildPawnPromotionMoves(moves, promoted, move_delta, false);

    return moves;
}

Move *BuildMoves(Move *moves, Square sq, BB targets, enum MoveType move_type) {
    while (targets)
        *(moves++) = move_make(sq, lsbpop(&targets), move_type);
    return moves;
}

Move *BuildJumperMoves(Move *moves, BB *mask, BB pieces, BB targets, enum MoveType move_type) {
    while (pieces) {
        Square piece = lsbpop(&pieces);
        moves = BuildMoves(moves, piece, targets & mask[piece], move_type);
    }
    return moves;
}

Move *GenerateSliderCaptures(Move *moves, BB *mask, BB pieces, BB empty, BB nus) {
    while (pieces) {
        Square piece = lsbpop(&pieces);
        BB unblocked = mask[piece];
        for (int offset = 1; unblocked && offset < 8; offset++) {
            BB captures = RINGS[piece][offset] & unblocked & nus;

            moves = BuildMoves(moves, piece, captures, Capture);

            BB blockers = RINGS[piece][offset] & unblocked & (~empty);
            while (blockers)
                unblocked &= ~RAYS[piece][lsbpop(&blockers)];
        }
    }
    return moves;
}

Move *GenerateSliderMoves(Move *moves, BB *mask, BB pieces, BB empty, BB nus) {
    while (pieces) {
        Square piece = lsbpop(&pieces);
        BB unblocked = mask[piece];
        for (int offset = 1; unblocked && offset < 8; offset++) {
            BB pot_moves = RINGS[piece][offset] & unblocked;
            BB quiet_moves = pot_moves & empty;
            BB capture_moves = pot_moves & nus;

            moves = BuildMoves(moves, piece, quiet_moves, Quiet);
            moves = BuildMoves(moves, piece, capture_moves, Capture);

            BB blockers = pot_moves & (~empty);
            while (blockers)
                unblocked &= ~RAYS[piece][lsbpop(&blockers)];
        }
    }
    return moves;
}

Move *GenerateCastlingMoves(Move *moves, Castling castling, Color turn, BB occ, BB attacks) {
    static const Square KING_POS[2] = {E1, E8};
    static const Square KING_CASTLE_POS[2] = {G1, G8};
    static const Square QUEEN_CASTLE_POS[2] = {C1, C8};
    static const BB KING_BLOCKERS[2] = {0x60lu, 0x6000000000000000lu};
    static const BB QUEEN_BLOCKERS[2] = {0xelu, 0xe00000000000000lu};
    static const BB QUEEN_ATTACKERS[2] = {0xclu, 0xc00000000000000lu};

    if ((castling & CASTLING_KING) && !(occ & KING_BLOCKERS[turn]) &&
        !(attacks & KING_BLOCKERS[turn]))
        *(moves++) = move_make(KING_POS[turn], KING_CASTLE_POS[turn], KingCastle);
    if ((castling & CASTLING_QUEEN) && !(occ & QUEEN_BLOCKERS[turn]) &&
        !(attacks & QUEEN_ATTACKERS[turn]))
        *(moves++) = move_make(KING_POS[turn], QUEEN_CASTLE_POS[turn], QueenCastle);

    return moves;
}

int GenerateCaptures(const Position *pos, Move *moves) {
    const Move *start = moves;
    const Color turn  = pos->turn;
    const BB us       = pos->colors[turn];
    const BB nus      = pos->colors[!turn];
    const BB empty    = ~(us | nus);
    const BB pawns    = us & (pos->pieces[PAWN]);
    const BB knights  = us & (pos->pieces[KNIGHT]);
    const BB bishops  = us & (pos->pieces[BISHOP] | pos->pieces[QUEEN]);
    const BB rooks    = us & (pos->pieces[ROOK]   | pos->pieces[QUEEN]);
    const BB kings    = us & (pos->pieces[KING]);

    moves = GeneratePawnCaptures(moves, turn, pawns, empty, nus, pos->ep_square);
    moves = BuildJumperMoves(moves, ATTACKS_KNIGHT, knights, nus, Capture);
    moves = BuildJumperMoves(moves, ATTACKS_KING, kings, nus, Capture);
    moves = GenerateSliderCaptures(moves, ATTACKS_BISHOP, bishops, empty, nus);
    moves = GenerateSliderCaptures(moves, ATTACKS_ROOK, rooks, empty, nus);

    return moves - start;
}

int GenerateMoves(const Position *pos, Move *moves) {
    const Move *start = moves;
    const Color turn  = pos->turn;
    const BB us       = pos->colors[turn];
    const BB nus      = pos->colors[!turn];
    const BB empty    = ~(us | nus);
    const BB pawns    = us & (pos->pieces[PAWN]);
    const BB knights  = us & (pos->pieces[KNIGHT]);
    const BB bishops  = us & (pos->pieces[BISHOP] | pos->pieces[QUEEN]);
    const BB rooks    = us & (pos->pieces[ROOK]   | pos->pieces[QUEEN]);
    const BB kings    = us & (pos->pieces[KING]);

    moves = GeneratePawnQuiet(moves, turn, pawns, empty);
    moves = GeneratePawnCaptures(moves, turn, pawns, empty, nus, pos->ep_square);

    moves = BuildJumperMoves(moves, ATTACKS_KNIGHT, knights, empty, Quiet);
    moves = BuildJumperMoves(moves, ATTACKS_KNIGHT, knights, nus, Capture);

    BB attacks = GenerateAttackBoard(pos, !turn);
    moves = BuildJumperMoves(moves, ATTACKS_KING, kings, empty & (~attacks), Quiet);
    moves = BuildJumperMoves(moves, ATTACKS_KING, kings, nus & (~attacks), Capture);

    // This checks whether any king is attacked, however it is only possible for the
    // current turns king to be under attack
    if (!(attacks & kings))
        moves = GenerateCastlingMoves(moves, pos->castling[turn], turn, ~empty, attacks);

    moves = GenerateSliderMoves(moves, ATTACKS_BISHOP, bishops, empty, nus);
    moves = GenerateSliderMoves(moves, ATTACKS_ROOK, rooks, empty, nus);

    return moves - start;
}
