#include <assert.h>
#include <ctype.h>
#include <stdlib.h>

#include "misc.h"
#include "utility.h"

unsigned int dist_horizontal(Square a, Square b) {
    assert(a >= A1 && a <= H8);
    assert(b >= A1 && b <= H8);
    return (unsigned int)abs((int)sq_file(a) - (int)sq_file(b));
}

unsigned int dist_vertical(Square a, Square b) {
    assert(a >= A1 && a <= H8);
    assert(b >= A1 && b <= H8);
    return (unsigned int)abs((int)sq_rank(a) - (int)sq_rank(b));
}

unsigned int dist_manhattan(Square a, Square b) {
    assert(a >= A1 && a <= H8);
    assert(b >= A1 && b <= H8);
    return dist_horizontal(a, b) + dist_vertical(a, b);
}

unsigned int dist_chebyshev(Square a, Square b) {
    assert(a >= A1 && a <= H8);
    assert(b >= A1 && b <= H8);
    return max(dist_horizontal(a, b), dist_vertical(a, b));
}

File sq_file(Square sq) {
    assert(sq >= A1 && sq <= H8);
    return sq % 8;
}

File sq_filec(char sq) {
    assert(sq >= A1 && sq <= H8);
    return sq - 'a';
}

Rank sq_rank(Square sq) {
    assert(sq >= A1 && sq <= H8);
    return (sq - sq % 8) / 8;
}

Rank sq_rankc(char sq) {
    assert(sq >= A1 && sq <= H8);
    return sq - '1';
}

Square sq_from(Rank rank, File file) {
    assert(rank >= RANK_1 && rank <= RANK_8);
    assert(file >= FILE_1 && file <= FILE_8);
    return 8 * rank + file;
}

Piece piece_from(char c) {
    c = tolower(c);
    switch (c) {
    case 'p':
        return PAWN;
    case 'n':
        return KNIGHT;
    case 'b':
        return BISHOP;
    case 'r':
        return ROOK;
    case 'q':
        return QUEEN;
    case 'k':
        return KING;
    }
    abort();
}

BB sbb(Square sq) {
    assert(sq >= A1 && sq <= H8);
    return 1ull << sq;
}

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
