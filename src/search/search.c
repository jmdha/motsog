#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chess/move.h"
#include "chess/move_gen.h"
#include "chess/position.h"
#include "eval/eval.h"
#include "search.h"
#include "misc.h"
#include "search/move_ordering.h"
#include "search/tt.h"

uint64_t NODES;

static int quiesce(
	const Position *pos,
	int alpha,
	int beta
) {
	int stand_pat = eval(pos, pos->turn);
	if (stand_pat >= beta)
		return beta;
	if (alpha < stand_pat)
		alpha = stand_pat;
	
	Move moves[MAX_MOVES];
	unsigned int scores[MAX_MOVES] = {0};
	const unsigned int count = generate_captures(pos, moves);
	order(moves, scores, count, pos, 0);
	
	Position new_pos;
	for (unsigned int i = 0; i < count; i++) {
		pick_move(moves, scores, count, i);
		apply(&new_pos, pos, moves[i]);
		if (is_king_safe(&new_pos, !new_pos.turn)) {
			int val = -quiesce(&new_pos, -beta, -alpha);
			if (val >= beta)
				return beta;
			if (val > alpha)
			    alpha = val;
		}
	}
	
	return alpha;
}

static int negamax(
	Move *best,
	const Position *pos,
	int depth,
	int ply,
	int alpha,
	int beta
) {
	if (depth == 0)
		return quiesce(pos, alpha, beta);
	if (is_threefold(pos))
		return 0;
	
	Move moves[MAX_MOVES];
	unsigned int scores[MAX_MOVES] = {0};
	const unsigned int count = generate_moves(pos, moves);
	if (!count) {
		if (!is_king_safe(pos, pos->turn))
			return -INT_MAX;
		else
			return 0;
	}
	order(moves, scores, count, pos, tt_probe(*pos->hash));
	
	Position new_pos;
	int b_val = -INT_MAX;
	Move best_child = moves[0];
	for (unsigned int i = 0; i < count; i++) {
		pick_move(moves, scores, count, i);
		apply(&new_pos, pos, moves[i]);
		if (is_king_safe(&new_pos, !new_pos.turn)) {
			NODES++;
			int val = -negamax(
					&best_child,
					&new_pos,
					depth - 1,
					ply + 1,
					-beta,
					-alpha
				  );
			if (val > b_val) {
				b_val = val;
				*best = moves[i];
				tt_store(*pos->hash, moves[i]);
			}
			if (val >= beta)
				return beta;
			if (val > alpha)
				alpha = val;
		}
	}
	
	return alpha;
}

void print_status(
	unsigned int depth,
	int          val,
	uint64_t     nodes,
	double       time_ms,
	Move         move
) {
	printf(
		"info depth %d score cp %d nps %.0f nodes %lu time %.0f hashfull %d pv ",
		depth,
		val / 21,
		(nodes / (double)time_ms) * 1000,
		nodes,
		time_ms,
		tt_hash_full()
	);
	move_print(move);
	printf("\n");
	fflush(stdout);
}

Move find_best_move(const Position *pos, unsigned int time_limit) {
	Move best;
	for (unsigned int depth = 1; depth < 256; depth++) {
		NODES = 0;
		const double t0 = time_ns();
		const int val = negamax(&best, pos, depth, 0, -INT_MAX, INT_MAX);
		const double time = time_ns() - t0; 
		const double time_ms = time / 1e6;
		print_status(depth, val, NODES, time_ms, best);
		if (NODES == 1 || abs(val) == INT_MAX || time_ms * 256 > time_limit)
			break;
	}
	return best;
}
