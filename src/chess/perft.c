#include "perft.h"
#include "chess/board.h"
#include "chess/position.h"
#include "move_gen.h"
#include "types.h"
#include <stdio.h>

uint64_t Perft(Board *board, int depth) {
    if (depth <= 0)
        return 1;
    Move moves[MAX_MOVES];
    int move_count = GenerateMoves(GetPosition(board), moves);

    uint64_t children = 0;
    for (int i = 0; i < move_count; i++) {
        ApplyMove(board, moves[i]);
        if (IsKingSafe(GetPosition(board), !GetPosition(board)->turn))
            children += Perft(board, depth - 1);
        UndoMove(board, moves[i]);
    }
    return children;
}

void PerftDivide(Board *board, int depth) {
    uint64_t total = 0;

    Move moves[MAX_MOVES];
    int move_count = GenerateMoves(GetPosition(board), moves);

    for (int i = 0; i < move_count; i++) {
        ApplyMove(board, moves[i]);
        if (IsKingSafe(GetPosition(board), !GetPosition(board)->turn)) {
            uint64_t children = Perft(board, depth - 1);
            PrintMove(moves[i]);
            printf(" %zu\n", children);
            total += children;
        }
        UndoMove(board, moves[i]);
    }

    printf("\n%lu\n", total);
}
