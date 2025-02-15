#pragma once

#include "chess/types.h"

void pick_move(Move moves[MAX_MOVES], unsigned int scores[MAX_MOVES], unsigned int count,
              unsigned int start);

void MVV(const Position *pos, Move moves[MAX_MOVES], unsigned int scores[MAX_MOVES],
            unsigned int count);

void order(Move tt_move, Move moves[MAX_MOVES], unsigned int scores[MAX_MOVES], unsigned int count);
