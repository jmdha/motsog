#include "move_ordering.h"
#include "chess/move.h"
#include "chess/position.h"
#include "misc.h"

void pick_move(Move moves[MAX_MOVES], unsigned int scores[MAX_MOVES], unsigned int count,
              unsigned int start) {
    for (unsigned int i = start + 1; i < count; i++) {
        if (scores[i] > scores[start]) {
            swap(&scores[i], &scores[start]);
            swap(&moves[i], &moves[start]);
        }
    }
}

const int VALUES[] = {10, 20, 30, 40, 50, 90, 0, 0};

void MVV(const Position *pos, Move moves[MAX_MOVES], unsigned int scores[MAX_MOVES],
            unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        const Move move = moves[i];
        const Square to = move_to(move);
        const Piece target = square_piece(pos, to);
        scores[i] += VALUES[target];
    }
}

void order(Move tt_move, Move moves[MAX_MOVES], unsigned int scores[MAX_MOVES], unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        if (moves[i] == tt_move) {
            scores[i] += 1000;
            continue;
        }
        const MoveType type = move_type(moves[i]);
        if (type == QPromotion)
            scores[i] += 100;
    }
}
