#include "types.h"
#include <stdint.h>

typedef uint64_t Hash;

void InitZobrist();

Hash FlipSquare(Hash hash, Square square, PieceType piece_type);
Hash FlipEnpassant(Hash hash, Square sq);
Hash FlipCastle(Hash hash, Color color, Castling castle);
Hash FlipTurn(Hash hash);
