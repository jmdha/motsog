#pragma once

#include "board.h"
#include "move.h"
#include "position.h"

int GenerateMoves(Position *pos, Move *moves);
int GenerateLegalMoves(Board *board, Move *moves);
