#pragma once

#include "board.h"
#include "move.h"
#include "position.h"

int GenerateCaptures(const Position *pos, Move *moves);
int GenerateMoves(const Position *pos, Move *moves);
int GenerateLegalMoves(Board *board, Move *moves);
