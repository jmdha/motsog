#pragma once

#include "board.h"

uint64_t Perft(Board *board, int depth);
void PerftDivide(Board *board, int depth);
