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

const int MVV_VALUES[] = {10, 20, 30, 40, 50, 90, 0, 0};

void MVV(const Position *pos, Move moves[MAX_MOVES], unsigned int scores[MAX_MOVES],
            unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        const Move move = moves[i];
        const Square to = move_to(move);
        const Piece target = square_piece(pos, to);
        scores[i] += MVV_VALUES[target];
    }
}

const int MVVLVA_VALUES[] = {
	15, 25, 35, 45, 55, 95, 0, 0,
	14, 24, 34, 44, 54, 94, 0, 0,
	13, 23, 33, 43, 53, 93, 0, 0,
	12, 22, 32, 42, 52, 92, 0, 0,
	11, 21, 31, 41, 51, 91, 0, 0,
	10, 20, 30, 40, 50, 90, 0, 0
};

void MVVLVA(const Position *pos, Move moves[MAX_MOVES], unsigned int scores[MAX_MOVES],
            unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        const Move move    = moves[i];
        const Square from  = move_from(move);
        const Square to    = move_to(move);
        const Piece piece = square_piece(pos, from);
        const Piece target = square_piece(pos, to);
        scores[i] += MVVLVA_VALUES[piece * 8 + target];
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
