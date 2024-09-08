#include "chess/types.h"

extern unsigned int PHASE[PIECE_COUNT];
extern const int VALUE_MG[6];
extern const int VALUE_EG[6];
extern int TABLE_MG[COLOR_COUNT][PIECE_COUNT][SQUARE_COUNT];
extern int TABLE_EG[COLOR_COUNT][PIECE_COUNT][SQUARE_COUNT];

void ValueInit(void);
