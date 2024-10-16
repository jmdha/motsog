#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "chess/move.h"
#include "chess/move_gen.h"
#include "chess/position.h"
#include "eval/eval.h"
#include "search.h"
#include "search/move_ordering.h"

unsigned int NODES;

static int Quiesce(const Position *pos, int alpha, int beta) {
    int stand_pat = Evaluate(pos, pos->turn);
    if (stand_pat >= beta)
        return beta;
    if (alpha < stand_pat)
        alpha = stand_pat;

    Move moves[MAX_MOVES];
    unsigned int scores[MAX_MOVES] = {0};
    const unsigned int count = GenerateCaptures(pos, moves);
    MVVLVA(pos, moves, scores, count);

    Position new_pos;
    for (unsigned int i = 0; i < count; i++) {
        PickMove(moves, scores, count, i);
        apply(&new_pos, pos, moves[i]);
        if (IsKingSafe(&new_pos, !new_pos.turn)) {
            int val = -Quiesce(&new_pos, -beta, -alpha);
            if (val >= beta)
                return beta;
            if (val > alpha)
                alpha = val;
        }
    }

    return alpha;
}

static int Negamax(Move *best, const Position *pos, unsigned int depth, int alpha, int beta) {
    if (depth == 0)
        return Quiesce(pos, alpha, beta);
    if (is_threefold(pos))
        return 0;
    NODES++;
    Move moves[MAX_MOVES];
    unsigned int scores[MAX_MOVES] = {0};
    const unsigned int count = GenerateMoves(pos, moves);
    MVVLVA(pos, moves, scores, count);
    if (!count) {
        if (!IsKingSafe(pos, pos->turn))
            return -INT_MAX;
        else
            return 0;
    }

    Position new_pos;
    int b_val = -INT_MAX;
    Move best_child = moves[0];
    for (unsigned int i = 0; i < count; i++) {
        PickMove(moves, scores, count, i);
        apply(&new_pos, pos, moves[i]);
        if (IsKingSafe(&new_pos, !new_pos.turn)) {
            int val = -Negamax(&best_child, &new_pos, depth - 1, -beta, -alpha);
            if (val > b_val) {
                b_val = val;
                *best = moves[i];
            }
            if (val >= beta)
                return beta;
            if (val > alpha)
                alpha = val;
        }
    }

    return alpha;
}

Move FindBestMove(const Position *pos, unsigned int time_limit) {
    Move best;
    for (unsigned int depth = 1; depth < 256; depth++) {
        NODES = 0;
        const clock_t t0 = clock();
        const int val = Negamax(&best, pos, depth, -INT_MAX, INT_MAX);
        const clock_t t1 = clock();
        const float seconds = (float)(t1 - t0) / CLOCKS_PER_SEC;
        const uint64_t ms = seconds * 1000;
        const uint64_t nps = (uint64_t)(NODES / seconds);
        printf("info depth %d score cp %d nps %lu nodes %u time %lu pv ", depth, val, nps,
               NODES, ms);
        move_print(best);
        printf("\n");
        fflush(stdout);
        if (ms > time_limit / 20 || abs(val) == INT_MAX)
            break;
    }
    return best;
}
