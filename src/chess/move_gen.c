#include "move_gen.h"
#include "attacks.h"
#include "bitboard.h"
#include "move.h"
#include "position.h"
#include <assert.h>
#include <stdbool.h>

typedef BB (*AttackFunc)(Square);

Move *BuildPawnMoves(Move *moves, BB targets, int delta, enum MoveType move_type) {
    while (targets) {
        Square dst = LSBPop(&targets);
        *(moves++) = MoveMake(dst - delta, dst, move_type);
    }
    return moves;
}

Move *BuildPawnPromotionMoves(Move *moves, BB targets, int delta, bool capture) {
    while (targets) {
        Square dst = LSBPop(&targets);
        *(moves++) = MoveMake(dst - delta, dst, NPromotion + 4 * capture);
        *(moves++) = MoveMake(dst - delta, dst, BPromotion + 4 * capture);
        *(moves++) = MoveMake(dst - delta, dst, RPromotion + 4 * capture);
        *(moves++) = MoveMake(dst - delta, dst, QPromotion + 4 * capture);
    }
    return moves;
}

Move *GeneratePawnMoves(Move *moves, Color turn, BB pawns, BB empty, BB nus, Square ep) {
    static const BB ADVANCED_ONCE[COLOR_COUNT] = {RANK_3, RANK_6};
    static const BB PROMOTED_RANK = RANK_8 | RANK_1;
    const int move_delta = turn == WHITE ? 8 : -8;
    const int left_capture_delta = turn == WHITE ? 7 : -9;
    const int right_capture_delta = turn == WHITE ? 9 : -7;

    BB advanced = ShiftUp(turn, pawns) & empty;
    BB advanced_double = ShiftUp(turn, advanced & ADVANCED_ONCE[turn]) & empty;
    BB promoted = advanced & PROMOTED_RANK;
    advanced ^= promoted;

    moves = BuildPawnMoves(moves, advanced, move_delta, Quiet);
    moves = BuildPawnMoves(moves, advanced_double, 2 * move_delta, DoublePawnPush);
    moves = BuildPawnPromotionMoves(moves, promoted, move_delta, false);

    // Remove those capture reached from opposite side of board
    // Do note, this is left/right from the perspective of white
    BB left_captures = nus & (((turn == WHITE) ? (pawns << 7) : (pawns >> 9)) & ~FILE_8);
    BB right_captures = nus & (((turn == WHITE) ? (pawns << 9) : (pawns >> 7)) & ~FILE_1);
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
    assert(PawnAttacks(SQUARE_NONE, !turn) == 0);
    BB ep_pawns = PawnAttacks(ep, !turn) & pawns;
    while (ep_pawns)
        *(moves++) = MoveMake(LSBPop(&ep_pawns), ep, EPCapture);

    return moves;
}

Move *BuildMoves(Move *moves, Square sq, BB targets, enum MoveType move_type) {
    while (targets)
        *(moves++) = MoveMake(sq, LSBPop(&targets), move_type);
    return moves;
}

Move *BuildJumperMoves(Move *moves, AttackFunc F, BB pieces, BB targets, enum MoveType move_type) {
    while (pieces) {
        Square piece = LSBPop(&pieces);
        moves = BuildMoves(moves, piece, targets & F(piece), move_type);
    }
    return moves;
}

Move *GenerateSliderMoves(Move *moves, AttackFunc F, BB pieces, BB empty, BB nus) {
    // TODO: Replace logic with *magic*
    while (pieces) {
        Square piece = LSBPop(&pieces);
        BB unblocked = F(piece);
        for (int offset = 1; unblocked && offset < 8; offset++) {
            BB ring = Ring(piece, offset);
            BB pot_moves = ring & unblocked;
            BB quiet_moves = pot_moves & empty;
            BB capture_moves = pot_moves & nus;

            moves = BuildMoves(moves, piece, quiet_moves, Quiet);
            moves = BuildMoves(moves, piece, capture_moves, Capture);

            BB blockers = pot_moves & (~empty);
            while (blockers)
                unblocked &= ~Ray(piece, LSBPop(&blockers));
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
        *(moves++) = MoveMake(KING_POS[turn], KING_CASTLE_POS[turn], KingCastle);
    if ((castling & CASTLING_QUEEN) && !(occ & QUEEN_BLOCKERS[turn]) &&
        !(attacks & QUEEN_ATTACKERS[turn]))
        *(moves++) = MoveMake(KING_POS[turn], QUEEN_CASTLE_POS[turn], QueenCastle);

    return moves;
}

int GenerateMoves(Position *pos, Move *moves) {
    const Move *start = moves;

    const Color turn = pos->turn;

    const BB us = pos->colors[turn];
    const BB nus = pos->colors[!turn];
    const BB empty = ~(us | nus);

    BB pawns = us & (pos->pieces[PAWN]);
    BB knights = us & (pos->pieces[KNIGHT]);
    BB bishops = us & (pos->pieces[BISHOP]);
    BB rooks = us & (pos->pieces[ROOK]);
    BB queens = us & (pos->pieces[QUEEN]);
    BB kings = us & (pos->pieces[KING]);
    assert(kings);

    // Combine sliders
    // This is allowed as moves store no information regarding sliders
    rooks |= queens;
    bishops |= queens;

    moves = GeneratePawnMoves(moves, turn, pawns, empty, nus, pos->ep_square);

    moves = BuildJumperMoves(moves, KnightAttacks, knights, empty, Quiet);
    moves = BuildJumperMoves(moves, KnightAttacks, knights, nus, Capture);

    BB attacks = GenerateAttackBoard(pos, !turn);
    moves = BuildJumperMoves(moves, KingAttacks, kings, empty & (~attacks), Quiet);
    moves = BuildJumperMoves(moves, KingAttacks, kings, nus & (~attacks), Capture);

    // This checks whether any king is attacked, however it is only possible for the
    // current turns king to be under attack
    if (!(attacks & kings))
        moves = GenerateCastlingMoves(moves, pos->castling[turn], turn, ~empty, attacks);

    moves = GenerateSliderMoves(moves, BishopAttacks, bishops, empty, nus);
    moves = GenerateSliderMoves(moves, RookAttacks, rooks, empty, nus);

    return moves - start;
}

int GenerateLegalMoves(Board *board, Move *moves) {
    const Move *start = moves;
    Move pseudo_moves[MAX_MOVES];
    int move_count = GenerateMoves(GetPosition(board), pseudo_moves);
    for (int i = 0; i < move_count; i++) {
        Move move = pseudo_moves[i];
        ApplyMove(board, move);
        if (IsKingSafe(GetPosition(board), !GetPosition(board)->turn))
            *(moves++) = move;
        UndoMove(board, move);
    }
    return moves - start;
}
