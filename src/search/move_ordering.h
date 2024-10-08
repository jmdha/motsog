#pragma once

#include "chess/types.h"

void PickMove(Move moves[MAX_MOVES], unsigned int scores[MAX_MOVES], unsigned int count,
              unsigned int start);

void MVVLVA(const Position *pos, Move moves[MAX_MOVES], unsigned int scores[MAX_MOVES],
            unsigned int count);
