#include <assert.h>
#include <stdbool.h>

#include "bit.h"
#include "masks.h"
#include "move.h"
#include "move_gen.h"
#include "position.h"
#include "utility.h"

Move *build_pawn_moves(Move *moves, BB targets, int delta, enum MoveType move_type) {
    while (targets) {
        Square dst = lsbpop(&targets);
        *(moves++) = move_make(dst - delta, dst, move_type);
    }
    return moves;
}

Move *build_pawn_promotion_moves(Move *moves, BB targets, int delta, bool capture) {
    while (targets) {
        Square dst = lsbpop(&targets);
        *(moves++) = move_make(dst - delta, dst, NPromotion + 4 * capture);
        *(moves++) = move_make(dst - delta, dst, BPromotion + 4 * capture);
        *(moves++) = move_make(dst - delta, dst, RPromotion + 4 * capture);
        *(moves++) = move_make(dst - delta, dst, QPromotion + 4 * capture);
    }
    return moves;
}

Move *generate_pawn_captures(Move *moves, Color turn, BB pawns, BB empty, BB nus, Square ep) {
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

    moves = build_pawn_moves(moves, left_captures, left_capture_delta, Capture);
    moves = build_pawn_moves(moves, right_captures, right_capture_delta, Capture);
    moves = build_pawn_promotion_moves(moves, left_promotion_captures, left_capture_delta, true);
    moves = build_pawn_promotion_moves(moves, right_promotion_captures, right_capture_delta, true);

    // If no EnPassant is allowed, i.e. a double pawn move was not done last turn
    // then the ep square is square none
    assert(ATTACKS_PAWN[!turn][SQUARE_NONE] == 0);
    BB ep_pawns = ATTACKS_PAWN[!turn][ep] & pawns;
    while (ep_pawns)
        *(moves++) = move_make(lsbpop(&ep_pawns), ep, EPCapture);

    return moves;
}

Move *generate_pawn_quiet(Move *moves, Color turn, BB pawns, BB empty) {
    static const BB ADVANCED_ONCE[COLOR_COUNT] = {RANK_3_BB, RANK_6_BB};
    static const BB PROMOTED_RANK = RANK_8_BB | RANK_1_BB;
    const int move_delta = turn == WHITE ? 8 : -8;

    BB advanced = shift_up(turn, pawns) & empty;
    BB advanced_double = shift_up(turn, advanced & ADVANCED_ONCE[turn]) & empty;
    BB promoted = advanced & PROMOTED_RANK;
    advanced ^= promoted;

    moves = build_pawn_moves(moves, advanced, move_delta, Quiet);
    moves = build_pawn_moves(moves, advanced_double, 2 * move_delta, DoublePawnPush);
    moves = build_pawn_promotion_moves(moves, promoted, move_delta, false);

    return moves;
}

Move *build_moves(Move *moves, Square sq, BB targets, enum MoveType move_type) {
    while (targets)
        *(moves++) = move_make(sq, lsbpop(&targets), move_type);
    return moves;
}

Move *build_jumper_moves(Move *moves, BB *mask, BB pieces, BB targets, enum MoveType move_type) {
    while (pieces) {
        Square piece = lsbpop(&pieces);
        moves = build_moves(moves, piece, targets & mask[piece], move_type);
    }
    return moves;
}

Move *generate_slider_captures(Move *moves, BB *mask, BB pieces, BB empty, BB nus) {
    while (pieces) {
        Square piece = lsbpop(&pieces);
        BB unblocked = mask[piece];
        for (int offset = 1; unblocked && offset < 8; offset++) {
            BB captures = RINGS[piece][offset] & unblocked & nus;

            moves = build_moves(moves, piece, captures, Capture);

            BB blockers = RINGS[piece][offset] & unblocked & (~empty);
            while (blockers)
                unblocked ^= RAYS[piece][lsbpop(&blockers)];
        }
    }
    return moves;
}

Move *generate_slider_moves(Move *moves, BB *mask, BB pieces, BB empty, BB nus) {
    while (pieces) {
        Square piece = lsbpop(&pieces);
        BB unblocked = mask[piece];
        for (int offset = 1; unblocked && offset < 8; offset++) {
            BB pot_moves = RINGS[piece][offset] & unblocked;
            BB quiet_moves = pot_moves & empty;
            BB capture_moves = pot_moves & nus;

            moves = build_moves(moves, piece, quiet_moves, Quiet);
            moves = build_moves(moves, piece, capture_moves, Capture);

            BB blockers = pot_moves & (~empty);
            while (blockers)
                unblocked ^= RAYS[piece][lsbpop(&blockers)];
        }
    }
    return moves;
}

Move *generate_castling_moves(Move *moves, Castling castling, Color turn, BB occ, BB attacks) {
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

int generate_captures(const Position *pos, Move *moves) {
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

    moves = generate_pawn_captures(moves, turn, pawns, empty, nus, pos->ep_square);
    moves = build_jumper_moves(moves, ATTACKS_KNIGHT, knights, nus, Capture);
    moves = build_jumper_moves(moves, ATTACKS_KING, kings, nus, Capture);
    moves = generate_slider_captures(moves, ATTACKS_BISHOP, bishops, empty, nus);
    moves = generate_slider_captures(moves, ATTACKS_ROOK, rooks, empty, nus);

    return moves - start;
}

int generate_moves(const Position *pos, Move *moves) {
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
    const BB attacks  = generate_attack_board(pos, !turn);

    // This checks whether any king is attacked, however it is only possible for the
    // current turns king to be under attack
    if (!(attacks & kings))
        moves = generate_castling_moves(moves, pos->castling[turn], turn, ~empty, attacks);

    moves = generate_pawn_quiet(moves, turn, pawns, empty);
    moves = generate_pawn_captures(moves, turn, pawns, empty, nus, pos->ep_square);

    moves = build_jumper_moves(moves, ATTACKS_KNIGHT, knights, empty, Quiet);
    moves = build_jumper_moves(moves, ATTACKS_KNIGHT, knights, nus, Capture);

    moves = generate_slider_moves(moves, ATTACKS_BISHOP, bishops, empty, nus);
    moves = generate_slider_moves(moves, ATTACKS_ROOK, rooks, empty, nus);

    moves = build_jumper_moves(moves, ATTACKS_KING, kings, empty & (~attacks), Quiet);
    moves = build_jumper_moves(moves, ATTACKS_KING, kings, nus & (~attacks), Capture);

    return moves - start;
}
