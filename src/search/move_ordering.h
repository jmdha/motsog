#pragma once

#include "chess/types.h"

void pick_move(
	Move moves[MAX_MOVES],
	unsigned int scores[MAX_MOVES],
	unsigned int count,
        unsigned int start
);

void order(
	Move moves[MAX_MOVES],
	unsigned int scores[MAX_MOVES],
	unsigned int count,
	const Position *pos,
	Move tt_move
);
