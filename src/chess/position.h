#pragma once

#include <stdbool.h>

#include "types.h"
#include "zobrist.h"

Position position(void);
Position import(const char *fen);
void position_free(Position *pos);

bool is_threefold(const Position *pos);

void apply(Position *restrict out, const Position *restrict pos, Move move);
void apply_moves(Position *pos, char *str);

Color GetSquareColor(const Position *pos, Square sq);
Piece GetPiece(const Position *pos, Square sq);
void PlacePiece(Position *pos, Color color, Square sq, Piece type);
void RemovePiece(Position *pos, Color color, Square sq, Piece type);
bool IsKingSafe(const Position *pos, Color color);
BB GenerateAttackBoard(const Position *pos, Color color);
