#pragma once

#include "types.h"
#include "zobrist.h"
#include <stdbool.h>

typedef struct Position {
    // A bitboard for each piece type
    BB pieces[PIECE_COUNT];
    // A bitboard for each color
    BB colors[COLOR_COUNT];
    // The castling rights of each player
    Castling castling[COLOR_COUNT];
    // The current turn
    Color turn;
    // The square in which en passant is allowed
    Square ep_square;
} Position;

Color GetSquareColor(const Position *pos, Square sq);
PieceType GetPiece(const Position *pos, Square sq);
void ClearPos(Position *pos);
void PlacePiece(Position *pos, Color color, Square sq, PieceType type);
void RemovePiece(Position *pos, Color color, Square sq, PieceType type);
void PrintPosition(Position *pos);
bool IsKingSafe(const Position *pos, Color color);
BB GenerateAttackBoard(const Position *pos, Color color);
