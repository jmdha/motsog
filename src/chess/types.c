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

Square SquareFrom(File file, Rank rank) { return 8 * rank + file; }
Square SquareFromChar(char column, char row) {
    return SquareFrom(ColumnFromChar(column), RowFromChar(row));
}
File ColumnFrom(Square sq) { return sq % 8; }
File ColumnFromChar(char c) {
    switch (c) {
    case 'A':
    case 'a':
        return FILE_1;
    case 'B':
    case 'b':
        return FILE_2;
    case 'C':
    case 'c':
        return FILE_3;
    case 'D':
    case 'd':
        return FILE_4;
    case 'E':
    case 'e':
        return FILE_5;
    case 'F':
    case 'f':
        return FILE_6;
    case 'G':
    case 'g':
        return FILE_7;
    case 'H':
    case 'h':
        return FILE_8;
    }
    abort();
}
Rank RowFrom(Square sq) { return (sq - sq % 8) / 8; }
Rank RowFromChar(char c) {
    switch (c) {
    case '1':
        return RANK_1;
    case '2':
        return RANK_2;
    case '3':
        return RANK_3;
    case '4':
        return RANK_4;
    case '5':
        return RANK_5;
    case '6':
        return RANK_6;
    case '7':
        return RANK_7;
    case '8':
        return RANK_8;
    }
    abort();
}

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
