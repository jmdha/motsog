#include "bitboard.h"
#include <assert.h>
#include <immintrin.h>

const BB RANKS[8] ={RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };
const BB FILES[8] ={FILE_1, FILE_2, FILE_3, FILE_4, FILE_5, FILE_6, FILE_7, FILE_8 };

int Popcount(BB bb) { return __builtin_popcountll(bb); }

int LSB(BB bb) {
    assert(bb);
    return __builtin_ctzll(bb);
}

int MSB(BB bb) {
    assert(bb);
    return __builtin_clzll(bb) ^ 63;
}

int LSBPop(BB *bb) {
    int lsb = LSB(*bb);
    *bb &= *bb - 1;
    return lsb;
}

int MSBPop(BB *bb) {
    int msb = MSB(*bb);
    *bb ^= 1ull << msb;
    return msb;
}

BB ShiftUp(Color color, BB bb) {
    if (color == WHITE)
        return bb << 8;
    else
        return bb >> 8;
}

bool Multiple(BB bb) {
    return (bb & (bb - 1)) != 0;
}
