#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "chess/move.h"
#include "chess/move_gen.h"
#include "chess/position.h"
#include "eval/eval.h"
#include "search.h"
#include "misc.h"
#include "search/move_ordering.h"
#include "search/tt.h"

uint64_t NODES;

static int quiesce(const Position *pos, int alpha, int beta) {
    int stand_pat = eval(pos, pos->turn);
    if (stand_pat >= beta)
        return beta;
    if (alpha < stand_pat)
        alpha = stand_pat;

    Move moves[MAX_MOVES];
    unsigned int scores[MAX_MOVES] = {0};
    const unsigned int count = generate_captures(pos, moves);
    MVV(pos, moves, scores, count);

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

static int negamax(Move *best, const Position *pos, int depth, int ply, int alpha, int beta) {
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
    const Move tt_move = tt_probe(*pos->hash);
    MVV(pos, moves, scores, count);
    order(tt_move, moves, scores, count);

    Position new_pos;
    int b_val       = -INT_MAX;
    Move best_child = moves[0];
    for (unsigned int i = 0; i < count; i++) {
        pick_move(moves, scores, count, i);
        apply(&new_pos, pos, moves[i]);
        if (is_king_safe(&new_pos, !new_pos.turn)) {
            NODES++;
            int val = -negamax(&best_child, &new_pos, depth - 1, ply + 1, -beta, -alpha);
            if (val > b_val) {
                b_val = val;
                *best = moves[i];
                tt_store(*pos->hash, moves[i]);
            }
            if (val >= beta) {
                return beta;
            }
            if (val > alpha) {
                alpha   = val;
            }
        }
    }

    return alpha;
}

Move find_best_move(const Position *pos, unsigned int time_limit) {
    Move best;
    for (unsigned int depth = 1; depth < 256; depth++) {
        NODES = 0;
        const uint64_t t0 = time_ms();
        const int val     = negamax(&best, pos, depth, 0, -INT_MAX, INT_MAX);
        const uint64_t t  = max(time_ms() - t0, 1u);
        printf(
            "info depth %d score cp %d nps %.0f nodes %lu time %lu hashfull %d pv ",
            depth, val / 21, (NODES / (double)t) * 1000, NODES, t, tt_hash_full()
        );
        move_print(best);
        printf("\n");
        fflush(stdout);
        if (NODES == 1 || abs(val) == INT_MAX || t * 256 > time_limit)
            break;
    }
    return best;
}
