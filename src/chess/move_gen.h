#pragma once

#include "types.h"

int GenerateCaptures(const Position *pos, Move *moves);
int GenerateMoves(const Position *pos, Move *moves);
int GenerateLegalMoves(const Position *pos, Move *moves);
