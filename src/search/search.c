#include <limits.h>
#include <stdio.h>
#include <time.h>

#include "chess/board.h"
#include "chess/move.h"
#include "chess/move_gen.h"
#include "chess/position.h"
#include "eval/eval.h"
#include "search.h"
#include "search/move_ordering.h"

static int Quiesce(Board *board, int alpha, int beta) {
    const Position *pos = GetPosition(board);
    int stand_pat = Evaluate(pos, pos->turn);
    if (stand_pat >= beta)
        return beta;
    if (alpha < stand_pat)
        alpha = stand_pat;

    Move moves[MAX_MOVES];
    const unsigned int count = GenerateCaptures(pos, moves);
    MVVLVA(pos, moves, count);

    for (unsigned int i = 0; i < count; i++) {
        bool valid = false;
        int val;
        ApplyMove(board, moves[i]);
        if (IsKingSafe(GetPosition(board), !GetPosition(board)->turn)) {
            val = -Quiesce(board, -beta, -alpha);
            valid = true;
        }
        UndoMove(board, moves[i]);
        if (valid) {
            if (val >= beta)
                return beta;
            if (val > alpha)
                alpha = val;
        }
    }

    return alpha;
}

static int Negamax(Board *board, unsigned int depth, int alpha, int beta) {
    const Position *pos = GetPosition(board);
    if (depth == 0)
        return Quiesce(board, alpha, beta);
    if (IsThreefold(board))
        return -1;
    Move moves[MAX_MOVES];
    const unsigned int count = GenerateMoves(pos, moves);
    MVVLVA(pos, moves, count);

    bool no_moves = true;
    for (unsigned int i = 0; i < count; i++) {
        bool valid = false;
        int val;
        ApplyMove(board, moves[i]);
        if (IsKingSafe(GetPosition(board), !GetPosition(board)->turn)) {
            val = -Negamax(board, depth - 1, -beta, -alpha);
            valid = true;
            no_moves = false;
        }
        UndoMove(board, moves[i]);
        if (valid) {
            if (val >= beta)
                return beta;
            if (val > alpha)
                alpha = val;
        }
    }
    if (no_moves) {
        if (!IsKingSafe(pos, pos->turn))
            return -INT_MAX + (100 - depth);
        else
            return -1;
    }

    return alpha;
}

static void Search(Move *best_move, int *best_score, Board *board, unsigned int depth) {
    *best_score = -INT_MAX;
    Move moves[MAX_MOVES];
    unsigned int count = GenerateLegalMoves(board, moves);
    for (unsigned int i = 0; i < count; i++) {
        const Move move = moves[i];
        ApplyMove(board, move);
        const int val = -Negamax(board, depth - 1, -INT_MAX, INT_MAX);
        UndoMove(board, move);
        if (val > *best_score) {
            *best_score = val;
            *best_move = move;
        }
    }
}

Move FindBestMove(Board *board, unsigned int time_limit) {
    const clock_t t0 = clock();
    const uint64_t starting_moves = board->moves;
    int best_score;
    Move best_move = 0;
    unsigned int depth = 1;
    while (true) {
        Search(&best_move, &best_score, board, depth);
        const clock_t t1 = clock();
        const float time = (float)(t1 - t0) / CLOCKS_PER_SEC;
        const uint64_t nodes = board->moves - starting_moves;
        const uint64_t nps = (uint64_t)(nodes / time);
        printf("info depth %d score cp %d nps %lu nodes %lu time %f\n", depth, best_score, nps,
               nodes, time),
            fflush(stdout);
        if (time * 1000 * 20 > time_limit / 20.0)
            break;
        depth++;
    }
    return best_move;
}
