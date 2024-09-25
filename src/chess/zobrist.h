#include "types.h"
#include <stdint.h>

void init_zobrist();

Hash flip_square(Hash hash, Square square, PieceType piece_type);
Hash flip_enpassant(Hash hash, Square sq);
Hash flip_castle(Hash hash, Color color, Castling castle);
Hash flip_turn(Hash hash);
