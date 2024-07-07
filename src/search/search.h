#pragma once

#include "chess/board.h"
#include "chess/move.h"

Move FindBestMove(Board *board, unsigned int time_limit);
