#include "bitboard.h"
#include <assert.h>
#include <immintrin.h>

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
