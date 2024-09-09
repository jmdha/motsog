#include "chess/types.h"

// values.x
// Various values used for evaluation purposes

void ValueInit(void);

unsigned int Phase(PieceType piece);
int ValueMG(Color color, PieceType piece, Square square);
int ValueEG(Color color, PieceType piece, Square square);
