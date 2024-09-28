#pragma once

#include "position.h"

uint64_t Perft(const Position *pos, int depth);
void PerftDivide(const Position *pos, int depth);
