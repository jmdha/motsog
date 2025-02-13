#pragma once

#include <stdbool.h>

#include "types.h"

Position position(void);
Position import(const char *fen);
void position_free(Position *pos);

bool is_threefold(const Position *pos);

void apply(Position *restrict out, const Position *restrict pos, Move move);
void apply_moves(Position *pos, char *str);

Color square_color(const Position *pos, Square sq);
Piece square_piece(const Position *pos, Square sq);
void place_piece(Position *pos, Color color, Square sq, Piece type);
void remove_piece(Position *pos, Color color, Square sq, Piece type);
bool is_king_safe(const Position *pos, Color color);
BB generate_attack_board(const Position *pos, Color color);
