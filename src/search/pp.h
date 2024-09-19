#pragma once

// pp.x
// Principal Principalities, or PP for short, stores the best moves for root'ish positions

#include "chess/move.h"

void PPInit();
void PPEnter(Move move);
void PPExit();
void PPStore(Move move, int eval);
Move PPRetrieve();
