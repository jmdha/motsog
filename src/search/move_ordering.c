#include "move_ordering.h"
#include "misc.h"

void PickMove(Move moves[MAX_MOVES], unsigned int scores[MAX_MOVES], unsigned int count, unsigned int start) {
    for (unsigned int i = start + 1; i < count; i++) {
        if (scores[i] > scores[start]) {
            swap(&scores[i], &scores[start]);
            swap(&moves[i], &moves[start]);
        }
    }
}

// clang-format off
// [PIECE][TARGET]
const unsigned int MVVALVA_VALUES[PIECE_COUNT][PIECE_COUNT + 1] = {
    {15, 25, 35, 45, 55, 0, 0},
    {14, 24, 34, 44, 54, 0, 0},
    {13, 23, 33, 43, 53, 0, 0},
    {12, 22, 32, 42, 52, 0, 0},
    {11, 21, 31, 41, 51, 0, 0},
    {10, 20, 30, 40, 50, 0, 0},
};
// clang-format on

void MVVLVA(const Position *pos, Move moves[MAX_MOVES], unsigned int scores[MAX_MOVES],
            unsigned int count) {
    for (unsigned int i = 0; i < count; i++) {
        const Move move = moves[i];
        const Square from = MoveFrom(move);
        const Square to = MoveTo(move);
        const Piece piece = GetPiece(pos, from);
        const Piece target = GetPiece(pos, to);
        scores[i] += MVVALVA_VALUES[piece][target];
    }
}
