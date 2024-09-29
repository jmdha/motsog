#pragma once

#include "chess/types.h"

typedef struct {
    unsigned int count;
    const Move *moves;
    const int *evals;
} PPResult;

void PPInit();
void PPEnter(Move move);
void PPExit();
void PPStore(Move move, int eval);
PPResult PPRetrieve();
Move PPBestMove();
void PPPrintPV();
