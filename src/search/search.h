#pragma once

#include "chess/move.h"
#include "chess/position.h"

Move FindBestMove(const Position *pos, unsigned int time_limit);
