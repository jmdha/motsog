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
#include "search/pp.h"

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

    for (unsigned int i = 0; i < count; i++) {
        PickMove(moves, scores, count, i);
        Position new_pos;
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

static int Negamax(const Position *pos, unsigned int depth, int alpha, int beta) {
    if (depth == 0)
        return Quiesce(pos, alpha, beta);
    Move moves[MAX_MOVES];
    const unsigned int count = GenerateMoves(pos, moves);
    if (!count) {
        if (!IsKingSafe(pos, pos->turn))
            return -INT_MAX;
        else
            return -1;
    }
    unsigned int scores[MAX_MOVES] = {0};
    MVVLVA(pos, moves, scores, count);

    for (unsigned int i = 0; i < count; i++) {
        PickMove(moves, scores, count, i);
        Position new_pos;
        apply(&new_pos, pos, moves[i]);
        if (IsKingSafe(&new_pos, !new_pos.turn)) {
            PPEnter(moves[i]);
            int val = -Negamax(&new_pos, depth - 1, -beta, -alpha);
            PPExit();
            if (val >= beta)
                return beta;
            PPStore(moves[i], val);
            if (val > alpha)
                alpha = val;
        }
    }

    return alpha;
}

Move FindBestMove(const Position *pos, unsigned int time_limit) {
    PPInit();
    const clock_t t0 = clock();
    for (unsigned int depth = 1; depth < 256; depth++) {
        const int val = Negamax(pos, depth, -INT_MAX, INT_MAX);
        const clock_t t1 = clock();
        const float seconds = (float)(t1 - t0) / CLOCKS_PER_SEC;
        const uint64_t ms = seconds * 1000;
        //const uint64_t nodes = board->moves - starting_moves;
        //const uint64_t nps = (uint64_t)(nodes / seconds);
        //printf("info depth %2d score cp %4d nps %8lu nodes %8lu time %5lu pv", depth, val, nps, nodes,
        //       ms);
        //PPPrintPV();
        //printf("\n");
        //fflush(stdout);
        if (ms > time_limit / 20 || abs(val) == INT_MAX)
            break;
    }
    return PPBestMove();
}
