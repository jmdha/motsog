#pragma once

#include "types.h"

uint64_t Perft(const Position *pos, int depth);
void PerftDivide(const Position *pos, int depth);
