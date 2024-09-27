#pragma once

#include <stdint.h>

#define WIDTH        8
#define HEIGHT       8
#define COLOR_COUNT  2
#define PIECE_COUNT  6
#define SQUARE_COUNT 64
#define MAX_PLY      128
#define MAX_MOVES    256

typedef uint64_t Hash;
typedef uint64_t BB;
typedef enum Color { WHITE, BLACK, COLOR_NONE } Color;
typedef enum PieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, PIECE_TYPE_NONE } PieceType;
typedef enum Castling { CASTLING_NONE, CASTLING_KING, CASTLING_QUEEN, CASTLING_BOTH } Castling;
typedef enum Column { COL_A, COL_B, COL_C, COL_D, COL_E, COL_F, COL_G, COL_H, COL_NONE } Column;
typedef enum Row { ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8, ROW_NONE } Row;
// clang-format off
typedef enum Square {
    A1,B1,C1,D1,E1,F1,G1,H1,
    A2,B2,C2,D2,E2,F2,G2,H2,
    A3,B3,C3,D3,E3,F3,G3,H3,
    A4,B4,C4,D4,E4,F4,G4,H4,
    A5,B5,C5,D5,E5,F5,G5,H5,
    A6,B6,C6,D6,E6,F6,G6,H6,
    A7,B7,C7,D7,E7,F7,G7,H7,
    A8,B8,C8,D8,E8,F8,G8,H8,
    SQUARE_NONE
} Square;
// clang-format on

extern const char *PIECE_CHARS[COLOR_COUNT];
extern const char *SQUARES[SQUARE_COUNT];

Square SquareFrom(Column col, Row row);
Square SquareFromChar(char column, char row);
Column ColumnFrom(Square sq);
Column ColumnFromChar(char c);
Row RowFrom(Square sq);
Row RowFromChar(char c);
PieceType CharToPieceType(char c);
