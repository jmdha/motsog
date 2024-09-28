#include <stdio.h>

#include "perft.h"
#include "chess/position.h"
#include "move_gen.h"
#include "types.h"

uint64_t Perft(const Position *pos, int depth) {
    if (depth <= 0)
        return 1;
    Move moves[MAX_MOVES];
    int move_count = GenerateMoves(pos, moves);

    uint64_t children = 0;
    for (int i = 0; i < move_count; i++) {
        Position new_pos;
        apply(&new_pos, pos, moves[i]);
        if (IsKingSafe(&new_pos, !new_pos.turn))
            children += Perft(&new_pos, depth - 1);
    }
    return children;
}

void PerftDivide(const Position *pos, int depth) {
    uint64_t total = 0;

    Move moves[MAX_MOVES];
    int move_count = GenerateMoves(pos, moves);

    for (int i = 0; i < move_count; i++) {
        Position new_pos;
        apply(&new_pos, pos, moves[i]);
        if (IsKingSafe(&new_pos, !new_pos.turn)) {
            uint64_t children = Perft(&new_pos, depth - 1);
            PrintMove(moves[i]);
            printf(" %zu\n", children);
            total += children;
        }
    }

    printf("\n%lu\n", total);
}
