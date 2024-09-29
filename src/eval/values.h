#include "chess/types.h"

void ValueInit(void);

unsigned int Phase(Piece piece);
int ValueMG(Color color, Piece piece, Square square);
int ValueEG(Color color, Piece piece, Square square);
