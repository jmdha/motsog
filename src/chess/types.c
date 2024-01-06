#include "types.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

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
