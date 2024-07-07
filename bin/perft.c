#include "chess/perft.h"
#include <stdio.h>
#include <stdlib.h>

#include "chess/board.h"
#include "misc.h"

int main(int argc, char **argv) {
    Init();
    if (argc < 3) {
        printf("Expected at least 2 arguments\n");
        abort();
    }
    unsigned int depth = atoi(argv[1]);
    char *fen = argv[2];
    Board board = ImportFEN(fen);
    if (argc > 3) {
        printf("Applying moves\n");
        char *moves = argv[3];
        ImportMoves(&board, moves);
    }
    PerftDivide(&board, depth);
    return 0;
}
