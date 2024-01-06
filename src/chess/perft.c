#include "perft.h"
#include "move_gen.h"
#include "types.h"
#include <stdio.h>

uint64_t Perft(Board *board, int depth) {
    Move moves[MAX_MOVES];
    int move_count = GenerateLegalMoves(board, moves);
    if (depth == 1)
        return move_count;

    uint64_t children = 0;
    for (int i = 0; i < move_count; i++) {
        ApplyMove(board, moves[i]);
        children += Perft(board, depth - 1);
        UndoMove(board, moves[i]);
    }

    return children;
}

void PerftDivide(Board *board, int depth) {
    uint64_t total = 0;

    Move moves[MAX_MOVES];
    int move_count = GenerateLegalMoves(board, moves);

    for (int i = 0; i < move_count; i++) {
        Move move = moves[i];
        ApplyMove(board, move);
        uint64_t children = Perft(board, depth - 1);
        PrintMove(move);
        printf(" %zu\n", children);
        total += children;
        UndoMove(board, move);
    }

    printf("%lu\n", total);
}
