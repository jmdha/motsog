#include "types.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

const char *PIECE_CHARS[COLOR_COUNT] = {"PNBRQK", "pnbrqk"};
// clang-format off
const char *SQUARES[SQUARE_COUNT] = {
    "a1","b1","c1","d1","e1","f1","g1","h1",
    "a2","b2","c2","d2","e2","f2","g2","h2",
    "a3","b3","c3","d3","e3","f3","g3","h3",
    "a4","b4","c4","d4","e4","f4","g4","h4",
    "a5","b5","c5","d5","e5","f5","g5","h5",
    "a6","b6","c6","d6","e6","f6","g6","h6",
    "a7","b7","c7","d7","e7","f7","g7","h7",
    "a8","b8","c8","d8","e8","f8","g8","h8"
};
// clang-format on

Piece CharToPieceType(char c) {
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
    default:
        printf("Unexpected piece char: %c\n", c);
        exit(1);
    }
}
