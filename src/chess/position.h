#pragma once

#include <stdbool.h>

#include "types.h"

// ALLOCATION

Position position(void);
Position import(const char *fen);
void     position_free(Position *pos);

// ACCESS

bool     is_threefold(const Position *pos);
bool     is_king_safe(const Position *pos, Color color);
Color    square_color(const Position *pos, Square sq);
Piece    square_piece(const Position *pos, Square sq);
BB       generate_attack_board(const Position *pos, Color color);

// MODIFIERS

void     apply(Position *restrict out, const Position *restrict pos, Move move);
void     apply_moves(Position *pos, char *str);
void     place_piece(Position *pos, Color color, Square sq, Piece type);
void     remove_piece(Position *pos, Color color, Square sq, Piece type);
