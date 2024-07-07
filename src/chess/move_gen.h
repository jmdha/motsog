#pragma once

#include "move.h"
#include "board.h"
#include "position.h"

int GenerateMoves(const Position *pos, Move *moves);
int GenerateLegalMoves(Board *board, Move *moves);
