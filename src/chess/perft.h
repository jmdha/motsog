#pragma once

#include "types.h"

uint64_t perft(const Position *pos, int depth);
void perft_divide(const Position *pos, int depth);
