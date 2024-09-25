#include "chess/types.h"

void ValueInit(void);

unsigned int Phase(PieceType piece);
int ValueMG(Color color, PieceType piece, Square square);
int ValueEG(Color color, PieceType piece, Square square);
