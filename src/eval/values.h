#pragma once

#include "chess/types.h"

extern int PHASE[PIECE_COUNT];
extern int16_t TABLE_MG[COLOR_COUNT][PIECE_COUNT][SQUARE_COUNT];
extern int16_t TABLE_EG[COLOR_COUNT][PIECE_COUNT][SQUARE_COUNT];

void init_values(void);
