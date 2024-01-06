#pragma once

#include "types.h"

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

static const Square KING_CORNER[2] = {H1, H8};
static const Square QUEEN_CORNER[2] = {A1, A8};

int Popcount(BB bb);
int LSB(BB bb);
int MSB(BB bb);
int LSBPop(BB *bb);
int MSBPop(BB *bb);
BB ShiftUp(Color color, BB bb);
