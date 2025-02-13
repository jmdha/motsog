#include <stdio.h>
#include <stdlib.h>

#include "chess/perft.h"
#include "chess/position.h"
#include "misc.h"

int main(int argc, char **argv) {
    Init();
    if (argc < 3) {
        printf("Expected at least 2 arguments\n");
        abort();
    }
    unsigned int depth = atoi(argv[1]);
    char *fen = argv[2];
    Position pos = import(fen);
    perft_divide(&pos, depth);
    Fini();
    return 0;
}
