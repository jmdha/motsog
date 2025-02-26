#include <string.h>

#include "chess/utility.h"
#include "masks.h"
#include "types.h"

BB  RAYS[SQUARE_COUNT][SQUARE_COUNT];
BB  XRAYS[SQUARE_COUNT][SQUARE_COUNT];
BB  BAB[SQUARE_COUNT][PIECE_COUNT];
BB  RINGS[SQUARE_COUNT][8];
BB  ATTACKS_PAWN[COLOR_COUNT][SQUARE_COUNT + 1];
BB  ATTACKS_KNIGHT[SQUARE_COUNT];
BB  ATTACKS_BISHOP[SQUARE_COUNT];
BB  ATTACKS_ROOK[SQUARE_COUNT];
BB  ATTACKS_QUEEN[SQUARE_COUNT];
BB  ATTACKS_KING[SQUARE_COUNT];
BB* ATTACKS[PIECE_COUNT] = {NULL, ATTACKS_KNIGHT, ATTACKS_BISHOP, ATTACKS_ROOK, ATTACKS_QUEEN, ATTACKS_KING};

int Valid(File file, Rank rank) {
    return file >= FILE_1 && file <= FILE_8 && rank >= RANK_1 && rank <= RANK_8;
}

void TrySet(BB *bb, File file, Rank rank) {
    if (Valid(file, rank))
        *bb |= sbb(sq_from(rank, file));
}

BB GenerateRay(Square from, Square to) {
    BB ray = 0;

    File col_from = sq_file(from);
    File col_to = sq_file(to);
    Rank row_from = sq_rank(from);
    Rank row_to = sq_rank(to);

    int x = (col_from == col_to) ? 0 : (col_from < col_to ? 1 : -1);
    int y = (row_from == row_to) ? 0 : (row_from < row_to ? 1 : -1);

    for (int i = 1; i < 8; i++)
        TrySet(&ray, col_from + i * x, row_from + i * y);

    return ray;
}

BB GenerateXRay(Square from, Square to) { return RAYS[from][to] & (~RAYS[to][from]) & (~sbb(to)); }

BB GenerateBAB(Square sq, Piece p) {
    if (sbb(sq) & CORNERS)
        return ATTACKS[p][sq] & (~RINGS[sq][7]);
    if (sbb(sq) & EDGE) {
        if (sbb(sq) & RANK_1_BB)
            return ATTACKS[p][sq] & (~(EDGE ^ RANK_1_BB));
        if (sbb(sq) & RANK_8_BB)
            return ATTACKS[p][sq] & (~(EDGE ^ RANK_8_BB));
        if (sbb(sq) & FILE_1_BB)
            return ATTACKS[p][sq] & (~(EDGE ^ FILE_1_BB));
        if (sbb(sq) & FILE_8_BB)
            return ATTACKS[p][sq] & (~(EDGE ^ FILE_8_BB));
    }

    return ATTACKS[p][sq] & (~EDGE);
}

void init_masks(void) {
    for (Square a = A1; a <= H8; a++)
        for (Square b = A1; b <= H8; b++) {
            RINGS[a][dist_chebyshev(a, b)] |= sbb(b);
            RAYS[a][b] = GenerateRay(a, b);
        }

    for (int c = 0; c < 2; c++) {
        const int offset = (c == WHITE) ? 1 : -1;
        for (Square sq = A1; sq <= H8; sq++) {
            TrySet(&ATTACKS_PAWN[c][sq], sq_file(sq) + 1, sq_rank(sq) + offset);
            TrySet(&ATTACKS_PAWN[c][sq], sq_file(sq) - 1, sq_rank(sq) + offset);
        }
        ATTACKS_PAWN[c][SQUARE_NONE] = 0;
    }

    for (Square sq = A1; sq <= H8; sq++) {
        for (int offset = 1; offset < 8; offset++) {
            TrySet(&ATTACKS_BISHOP[sq], sq_file(sq) + offset, sq_rank(sq) + offset);
            TrySet(&ATTACKS_BISHOP[sq], sq_file(sq) + offset, sq_rank(sq) - offset);
            TrySet(&ATTACKS_BISHOP[sq], sq_file(sq) - offset, sq_rank(sq) + offset);
            TrySet(&ATTACKS_BISHOP[sq], sq_file(sq) - offset, sq_rank(sq) - offset);
            TrySet(&ATTACKS_ROOK[sq], sq_file(sq) + offset, sq_rank(sq));
            TrySet(&ATTACKS_ROOK[sq], sq_file(sq) - offset, sq_rank(sq));
            TrySet(&ATTACKS_ROOK[sq], sq_file(sq), sq_rank(sq) + offset);
            TrySet(&ATTACKS_ROOK[sq], sq_file(sq), sq_rank(sq) - offset);
        }
        ATTACKS_QUEEN[sq] = ATTACKS_BISHOP[sq] | ATTACKS_ROOK[sq];
    }

    for (Square sq = A1; sq <= H8; sq++) {
        ATTACKS_KING[sq] = RINGS[sq][1];
        ATTACKS_KNIGHT[sq] = RINGS[sq][2] & (~ATTACKS_QUEEN[sq]);
    }

    for (Square from = A1; from <= H8; from++)
        for (Square to = A1; to <= H8; to++)
            XRAYS[from][to] = GenerateXRay(from, to);
    for (Piece p = KNIGHT; p <= KING; p++)
        for (Square sq = A1; sq <= H8; sq++)
            BAB[sq][p] = GenerateBAB(sq, p);
}
