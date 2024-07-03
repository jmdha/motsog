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

BB ToBB(Square sq) { return 1ull << sq; }
Square SquareFrom(Column col, Row row) { return 8 * row + col; }
Square SquareFromChar(char column, char row) {
    return SquareFrom(ColumnFromChar(column), RowFromChar(row));
}
Column ColumnFrom(Square sq) { return sq % 8; }
Column ColumnFromChar(char c) {
    switch (c) {
    case 'A':
    case 'a':
        return COL_A;
    case 'B':
    case 'b':
        return COL_B;
    case 'C':
    case 'c':
        return COL_C;
    case 'D':
    case 'd':
        return COL_D;
    case 'E':
    case 'e':
        return COL_E;
    case 'F':
    case 'f':
        return COL_F;
    case 'G':
    case 'g':
        return COL_G;
    case 'H':
    case 'h':
        return COL_H;
    }
    return COL_NONE;
}
Row RowFrom(Square sq) { return (sq - sq % 8) / 8; }
Row RowFromChar(char c) {
    switch (c) {
    case '1':
        return ROW_1;
    case '2':
        return ROW_2;
    case '3':
        return ROW_3;
    case '4':
        return ROW_4;
    case '5':
        return ROW_5;
    case '6':
        return ROW_6;
    case '7':
        return ROW_7;
    case '8':
        return ROW_8;
    }
    return ROW_NONE;
}

PieceType CharToPieceType(char c) {
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
