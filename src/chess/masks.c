#include "masks.h"
#include "chess/utility.h"
#include "types.h"
#include <stdlib.h>

BB RAYS[SQUARE_COUNT][SQUARE_COUNT];
BB XRAYS[SQUARE_COUNT][SQUARE_COUNT];
BB BAB[PIECE_COUNT][SQUARE_COUNT];
BB RINGS[SQUARE_COUNT][8];
BB ATTACKS_PAWN[COLOR_COUNT][SQUARE_COUNT + 1];
BB ATTACKS_KNIGHT[SQUARE_COUNT];
BB ATTACKS_BISHOP[SQUARE_COUNT];
BB ATTACKS_ROOK[SQUARE_COUNT];
BB ATTACKS_KING[SQUARE_COUNT];

BB ray(Square from, Square to) { return RAYS[from][to]; }
BB xray(Square from, Square to) { return XRAYS[from][to]; }
BB ring(Square sq, int offset) { return RINGS[sq][offset]; }
BB bab(Square sq, Piece p) { return BAB[p][sq]; }
BB attacks_pawn(Square sq, Color color) { return ATTACKS_PAWN[color][sq]; }
BB attacks_knight(Square sq) { return ATTACKS_KNIGHT[sq]; }
BB attacks_bishop(Square sq) { return ATTACKS_BISHOP[sq]; }
BB attacks_rook(Square sq) { return ATTACKS_ROOK[sq]; }
BB attacks_king(Square sq) { return ATTACKS_KING[sq]; }
BB attacks(Square sq, Piece p) {
    switch (p) {
    case KNIGHT:
        return attacks_knight(sq);
    case BISHOP:
        return attacks_bishop(sq);
    case ROOK:
        return attacks_rook(sq);
    case QUEEN:
        return attacks_bishop(sq) | attacks_rook(sq);
    case KING:
        return attacks_king(sq);
    case PAWN:
    case PIECE_TYPE_NONE:
        abort();
    }
    abort();
}

int Valid(Column col, Row row) {
    return col >= COL_A && col <= COL_H && row >= ROW_1 && row <= ROW_8;
}

void TrySet(BB *bb, Column col, Row row) {
    if (Valid(col, row))
        *bb |= sbb(SquareFrom(col, row));
}

BB GenerateRay(Square from, Square to) {
    BB ray = 0;

    Column col_from = ColumnFrom(from);
    Column col_to = ColumnFrom(to);
    Row row_from = RowFrom(from);
    Row row_to = RowFrom(to);

    int x = (col_from == col_to) ? 0 : (col_from < col_to ? 1 : -1);
    int y = (row_from == row_to) ? 0 : (row_from < row_to ? 1 : -1);

    for (int i = 1; i < 8; i++)
        TrySet(&ray, col_from + i * x, row_from + i * y);

    return ray;
}

BB GenerateXRay(Square from, Square to) { return ray(from, to) & (~ray(to, from)) & (~sbb(to)); }

BB GenerateBAB(Square sq, Piece p) {
    if (sbb(sq) & CORNERS)
        return attacks(sq, p) & (~ring(sq, 7));
    if (sbb(sq) & EDGE) {
        if (sbb(sq) & RANK_1)
            return attacks(sq, p) & (~(EDGE ^ RANK_1));
        if (sbb(sq) & RANK_8)
            return attacks(sq, p) & (~(EDGE ^ RANK_8));
        if (sbb(sq) & FILE_1)
            return attacks(sq, p) & (~(EDGE ^ FILE_1));
        if (sbb(sq) & FILE_8)
            return attacks(sq, p) & (~(EDGE ^ FILE_8));
    }

    return attacks(sq, p) & (~EDGE);
}

BB GenerateRing(Square sq, unsigned int offset) {
    BB ring = 0;

    for (Square x = A1; x <= H8; x++)
        if ((dist_vertical(sq, x) == offset && dist_horizontal(sq, x) <= offset) ||
            (dist_horizontal(sq, x) == offset && dist_vertical(sq, x) <= offset))
            ring |= sbb(x);

    return ring;
}

void init_masks(void) {
    for (int c = 0; c < 2; c++) {
        const int offset = (c == WHITE) ? 1 : -1;
        for (Square sq = A1; sq <= H8; sq++) {
            TrySet(&ATTACKS_PAWN[c][sq], ColumnFrom(sq) + 1, RowFrom(sq) + offset);
            TrySet(&ATTACKS_PAWN[c][sq], ColumnFrom(sq) - 1, RowFrom(sq) + offset);
        }
        ATTACKS_PAWN[c][SQUARE_NONE] = 0;
    }

    const int KNIGHT_DELTA[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                                    {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
    const int KING_DELTA[8][2] = {{0, -1}, {0, 1},  {-1, -1}, {-1, 0},
                                  {-1, 1}, {1, -1}, {1, 0},   {1, 1}};

    // Generate jump moves
    for (Square sq = A1; sq <= H8; sq++) {
        for (int dir = 0; dir < 8; dir++) {
            TrySet(&ATTACKS_KNIGHT[sq], ColumnFrom(sq) + KNIGHT_DELTA[dir][0],
                   RowFrom(sq) + KNIGHT_DELTA[dir][1]);
            TrySet(&ATTACKS_KING[sq], ColumnFrom(sq) + KING_DELTA[dir][0],
                   RowFrom(sq) + KING_DELTA[dir][1]);
        }
    }

    for (Square sq = A1; sq <= H8; sq++) {
        for (int offset = 1; offset < 8; offset++) {
            TrySet(&ATTACKS_BISHOP[sq], ColumnFrom(sq) + offset, RowFrom(sq) + offset);
            TrySet(&ATTACKS_BISHOP[sq], ColumnFrom(sq) + offset, RowFrom(sq) - offset);
            TrySet(&ATTACKS_BISHOP[sq], ColumnFrom(sq) - offset, RowFrom(sq) + offset);
            TrySet(&ATTACKS_BISHOP[sq], ColumnFrom(sq) - offset, RowFrom(sq) - offset);
            TrySet(&ATTACKS_ROOK[sq], ColumnFrom(sq) + offset, RowFrom(sq));
            TrySet(&ATTACKS_ROOK[sq], ColumnFrom(sq) - offset, RowFrom(sq));
            TrySet(&ATTACKS_ROOK[sq], ColumnFrom(sq), RowFrom(sq) + offset);
            TrySet(&ATTACKS_ROOK[sq], ColumnFrom(sq), RowFrom(sq) - offset);
        }
    }

    for (Square from = A1; from <= H8; from++)
        for (Square to = A1; to <= H8; to++)
            RAYS[from][to] = GenerateRay(from, to);

    for (Square from = A1; from <= H8; from++)
        for (Square to = A1; to <= H8; to++)
            XRAYS[from][to] = GenerateXRay(from, to);

    for (Square sq = A1; sq <= H8; sq++) {
        for (int offset = 1; offset < 8; offset++) {
            RINGS[sq][offset] = GenerateRing(sq, offset);
        }
    }
    for (Piece p = KNIGHT; p <= KING; p++)
        for (Square sq = A1; sq <= H8; sq++)
            BAB[p][sq] = GenerateBAB(sq, p);
}
