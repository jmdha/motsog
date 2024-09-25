#include "masks.h"
#include "types.h"
#include <stdlib.h>

BB RAYS[SQUARE_COUNT][SQUARE_COUNT];
BB XRAYS[SQUARE_COUNT][SQUARE_COUNT];
BB BAB_[PIECE_COUNT][SQUARE_COUNT];
BB RINGS[SQUARE_COUNT][8];
BB PAWN_ATTACKS[COLOR_COUNT][SQUARE_COUNT + 1];
BB KNIGHT_ATTACKS[SQUARE_COUNT];
BB BISHOP_ATTACKS[SQUARE_COUNT];
BB ROOK_ATTACKS[SQUARE_COUNT];
BB KING_ATTACKS[SQUARE_COUNT];

BB Ray(Square from, Square to) { return RAYS[from][to]; }
BB XRay(Square from, Square to) { return XRAYS[from][to]; }
BB Ring(Square sq, int offset) { return RINGS[sq][offset]; }
BB BAB(Square sq, PieceType p) { return BAB_[p][sq]; }
BB PawnAttacks(Square sq, Color color) { return PAWN_ATTACKS[color][sq]; }
BB KnightAttacks(Square sq) { return KNIGHT_ATTACKS[sq]; }
BB BishopAttacks(Square sq) { return BISHOP_ATTACKS[sq]; }
BB RookAttacks(Square sq) { return ROOK_ATTACKS[sq]; }
BB KingAttacks(Square sq) { return KING_ATTACKS[sq]; }
BB Attacks(Square sq, PieceType p) {
    switch (p) {
    case KNIGHT:
        return KnightAttacks(sq);
    case BISHOP:
        return BishopAttacks(sq);
    case ROOK:
        return RookAttacks(sq);
    case QUEEN:
        return BishopAttacks(sq) | RookAttacks(sq);
    case KING:
        return KingAttacks(sq);
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
        *bb |= ToBB(SquareFrom(col, row));
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

BB GenerateXRay(Square from, Square to) { return Ray(from, to) & (~Ray(to, from)) & (~ToBB(to)); }

BB GenerateBAB(Square sq, PieceType p) {
    BB attacks = Attacks(sq, p);

    if (ToBB(sq) & CORNERS)
        return attacks & (~Ring(sq, 7));
    if (ToBB(sq) & EDGE) {
        if (ToBB(sq) & RANK_1)
            return attacks & (~(EDGE ^ RANK_1));
        if (ToBB(sq) & RANK_8)
            return attacks & (~(EDGE ^ RANK_8));
        if (ToBB(sq) & FILE_1)
            return attacks & (~(EDGE ^ FILE_1));
        if (ToBB(sq) & FILE_8)
            return attacks & (~(EDGE ^ FILE_8));
    }

    return attacks & (~EDGE);
}

BB GenerateRing(Square sq, int offset) {
    BB ring = 0;

    static const int DIRECTIONS[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}};
    static const int PROBES[4][2][2] = {
        {{1, 0}, {-1, 0}},
        {{1, 0}, {-1, 0}},
        {{0, 1}, {0, -1}},
        {{0, 1}, {0, -1}},
    };

    const Column col = ColumnFrom(sq);
    const Row row = RowFrom(sq);

    for (int i = 0; i < 4; i++) {
        TrySet(&ring, col + offset * DIRECTIONS[i][0], row + offset * DIRECTIONS[i][1]);
        for (int probe = 0; probe < 2; probe++) {
            for (int o = 1; o <= offset; o++)
                TrySet(&ring, col + offset * DIRECTIONS[i][0] + o * PROBES[i][probe][0],
                       row + offset * DIRECTIONS[i][1] + o * PROBES[i][probe][1]);
        }
    }

    return ring;
}

void InitMasks(void) {
    for (int c = 0; c < 2; c++) {
        const int offset = (c == WHITE) ? 1 : -1;
        for (Square sq = A1; sq <= H8; sq++) {
            TrySet(&PAWN_ATTACKS[c][sq], ColumnFrom(sq) + 1, RowFrom(sq) + offset);
            TrySet(&PAWN_ATTACKS[c][sq], ColumnFrom(sq) - 1, RowFrom(sq) + offset);
        }
        PAWN_ATTACKS[c][SQUARE_NONE] = 0;
    }

    const int KNIGHT_DELTA[8][2] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                                    {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};
    const int KING_DELTA[8][2] = {{0, -1}, {0, 1},  {-1, -1}, {-1, 0},
                                  {-1, 1}, {1, -1}, {1, 0},   {1, 1}};

    // Generate jump moves
    for (Square sq = A1; sq <= H8; sq++) {
        for (int dir = 0; dir < 8; dir++) {
            TrySet(&KNIGHT_ATTACKS[sq], ColumnFrom(sq) + KNIGHT_DELTA[dir][0],
                   RowFrom(sq) + KNIGHT_DELTA[dir][1]);
            TrySet(&KING_ATTACKS[sq], ColumnFrom(sq) + KING_DELTA[dir][0],
                   RowFrom(sq) + KING_DELTA[dir][1]);
        }
    }

    for (Square sq = A1; sq <= H8; sq++) {
        for (int offset = 1; offset < 8; offset++) {
            TrySet(&BISHOP_ATTACKS[sq], ColumnFrom(sq) + offset, RowFrom(sq) + offset);
            TrySet(&BISHOP_ATTACKS[sq], ColumnFrom(sq) + offset, RowFrom(sq) - offset);
            TrySet(&BISHOP_ATTACKS[sq], ColumnFrom(sq) - offset, RowFrom(sq) + offset);
            TrySet(&BISHOP_ATTACKS[sq], ColumnFrom(sq) - offset, RowFrom(sq) - offset);
            TrySet(&ROOK_ATTACKS[sq], ColumnFrom(sq) + offset, RowFrom(sq));
            TrySet(&ROOK_ATTACKS[sq], ColumnFrom(sq) - offset, RowFrom(sq));
            TrySet(&ROOK_ATTACKS[sq], ColumnFrom(sq), RowFrom(sq) + offset);
            TrySet(&ROOK_ATTACKS[sq], ColumnFrom(sq), RowFrom(sq) - offset);
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
    for (PieceType p = KNIGHT; p <= KING; p++)
        for (Square sq = A1; sq <= H8; sq++)
            BAB_[p][sq] = GenerateBAB(sq, p);
}

const BB RANKS[8] ={RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };
const BB FILES[8] ={FILE_1, FILE_2, FILE_3, FILE_4, FILE_5, FILE_6, FILE_7, FILE_8 };
