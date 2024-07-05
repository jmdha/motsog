#pragma once

#include "move.h"
#include "board.h"
#include "position.h"

int GenerateMoves(Position *pos, Move *moves);
int GenerateLegalMoves(Board *board, Move *moves);
