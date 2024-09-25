#pragma once

#include "types.h"

void InitMasks(void);

BB Ray(Square from, Square to);         // Has bits set between "from" and "to", inclusive
BB XRay(Square from, Square to);        // Has bits set from "to" onwards, exclusive
BB Ring(Square sq, int offset);         // Has bits set in a ring around "sq"
BB PawnAttacks(Square sq, Color color); // Denotes pawn attacks for an empty chess board
BB KnightAttacks(Square sq);            // Denotes knight attacks for an empty chess board
BB BishopAttacks(Square sq);            // Denotes bishop attacks for an empty chess board
BB RookAttacks(Square sq);              // Denotes rook attacks for an empty chess board
BB KingAttacks(Square sq);              // Denotes king attacks for an empty chess board
BB Attacks(Square sq, PieceType p);     // Simply a switch between individual attack functions
BB BAB(Square sq, PieceType p);         // Has bits set as "Attacks" excluding the edge

// TODO: Clean the following

enum Rank {
    RANK_1 = 0xffllu,
    RANK_2 = 0xff00llu,
    RANK_3 = 0xff0000llu,
    RANK_4 = 0xff000000llu,
    RANK_5 = 0xff00000000llu,
    RANK_6 = 0xff0000000000llu,
    RANK_7 = 0xff000000000000llu,
    RANK_8 = 0xff00000000000000llu,
};

enum File {
    FILE_1 = 0x101010101010101llu,
    FILE_2 = 0x202020202020202llu,
    FILE_3 = 0x404040404040404llu,
    FILE_4 = 0x808080808080808llu,
    FILE_5 = 0x1010101010101010llu,
    FILE_6 = 0x2020202020202020llu,
    FILE_7 = 0x4040404040404040llu,
    FILE_8 = 0x8080808080808080llu,
};

#define CORNERS (ToBB(A1) | ToBB(A8) | ToBB(H1) | ToBB(H8))
#define CORNER_A (ToBB(A1) | ToBB(A8))
#define CORNER_H (ToBB(H1) | ToBB(H8))
#define EDGE (RANK_1 | RANK_8 | FILE_1 | FILE_8)

extern const BB RANKS[8];
extern const BB FILES[8];

static const Square KING_CORNER[2] = {H1, H8};
static const Square QUEEN_CORNER[2] = {A1, A8};
