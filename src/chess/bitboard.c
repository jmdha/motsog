#include "bitboard.h"
#include <assert.h>
#include <immintrin.h>

const BB RANKS[8] ={RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };
const BB FILES[8] ={FILE_1, FILE_2, FILE_3, FILE_4, FILE_5, FILE_6, FILE_7, FILE_8 };

BB ShiftUp(Color color, BB bb) {
    if (color == WHITE)
        return bb << 8;
    else
        return bb >> 8;
}
