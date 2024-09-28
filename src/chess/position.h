#pragma once

#include <stdbool.h>

#include "types.h"
#include "zobrist.h"

Position position(void);
Position import(const char *fen);

void apply(Position *out, const Position *pos, Move move);
void apply_moves(Position *pos, char *str);

Color GetSquareColor(const Position *pos, Square sq);
PieceType GetPiece(const Position *pos, Square sq);
void PlacePiece(Position *pos, Color color, Square sq, PieceType type);
void RemovePiece(Position *pos, Color color, Square sq, PieceType type);
void PrintPosition(Position *pos);
bool IsKingSafe(const Position *pos, Color color);
BB GenerateAttackBoard(const Position *pos, Color color);
