#include <assert.h>

#include "utility.h"

BB shift_up(Color color, BB bb) {
    assert(color == WHITE || color == BLACK);
    if (color == WHITE)
        return bb << 8;
    else
        return bb >> 8;
}

BB shift_down(Color color, BB bb) {
    assert(color == WHITE || color == BLACK);
    if (color == WHITE)
        return bb >> 8;
    else
        return bb << 8;
}
