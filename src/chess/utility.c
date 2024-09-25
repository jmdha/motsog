#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include "misc.h"
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

unsigned int dist_horizontal(Square a, Square b) {
    assert(a >= A1 && a <= H8);
    assert(b >= A1 && b <= H8);
    return (unsigned int)abs((int)ColumnFrom(a) - (int)ColumnFrom(b));
}

unsigned int dist_vertical(Square a, Square b) {
    assert(a >= A1 && a <= H8);
    assert(b >= A1 && b <= H8);
    return (unsigned int)abs((int)RowFrom(a) - (int)RowFrom(b));
}

unsigned int dist_min(Square a, Square b) {
    assert(a >= A1 && a <= H8);
    assert(b >= A1 && b <= H8);
    return min(dist_horizontal(a, b), dist_vertical(a, b));
}

unsigned int dist_max(Square a, Square b) {
    assert(a >= A1 && a <= H8);
    assert(b >= A1 && b <= H8);
    return max(dist_horizontal(a, b), dist_vertical(a, b));
}

unsigned int dist_euclidean(Square a, Square b) {
    assert(a >= A1 && a <= H8);
    assert(b >= A1 && b <= H8);
    return sqrt(pow(dist_horizontal(a, b), 2) + pow(dist_vertical(a, b), 2));
}

unsigned int dist_manhattan(Square a, Square b) {
    assert(a >= A1 && a <= H8);
    assert(b >= A1 && b <= H8);
    return dist_horizontal(a, b) + dist_vertical(a, b);
}
