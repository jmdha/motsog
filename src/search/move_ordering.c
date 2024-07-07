#include <stdio.h>
#include <string.h>

#include "move_ordering.h"

inline void memswap(void *restrict buffer, void *l, void *r, size_t size) {
    memcpy(buffer, l, size);
    memmove(l, r, size);
    memcpy(r, buffer, size);
}

#define swap(a, b)                                                                                 \
    memswap(&(struct {                                                                             \
        _Static_assert(sizeof *(a) == sizeof *(b), "arguments of swap must have same size");       \
        char _[sizeof *(a)];                                                                       \
    }){0},                                                                                         \
            (a), (b), sizeof *(a))

// clang-format off
const unsigned int MVVALVA_VALUES[PIECE_COUNT][PIECE_COUNT + 1] = {
    {15, 25, 35, 45, 55, 0, 0},
    {14, 24, 34, 44, 54, 0, 0},
    {13, 23, 33, 43, 53, 0, 0},
    {12, 22, 32, 42, 52, 0, 0},
    {11, 21, 31, 41, 51, 0, 0},
    {10, 20, 30, 40, 50, 0, 0},
};
// clang-format on

void MVVLVA(const Position *pos, Move moves[MAX_MOVES], unsigned int count) {
    unsigned int values[MAX_MOVES];
    for (unsigned int i = 0; i < count; i++) {
        const Move move = moves[i];
        const Square from = MoveFrom(move);
        const Square to = MoveTo(move);
        const PieceType piece = GetPiece(pos, from);
        const PieceType target = GetPiece(pos, to);
        values[i] = MVVALVA_VALUES[piece][target];
        for (unsigned int t = 0; t < i; t++) {
            if (values[i] > values[t]) {
                for (unsigned int j = t; j < i - 1; j++) {
                    swap(&values[j], &values[j + 1]);
                    swap(&moves[j], &moves[j + 1]);
                }
                break;
            }
        }
    }
}
