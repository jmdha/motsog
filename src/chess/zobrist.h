#pragma once

#include <stdint.h>

#include "types.h"

void init_zobrist();

Hash flip_square(Hash hash, Square square, Piece piece_type);
Hash flip_enpassant(Hash hash, Square sq);
Hash flip_castle(Hash hash, Color color, Castling castle);
Hash flip_turn(Hash hash);
