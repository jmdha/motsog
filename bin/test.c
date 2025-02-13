#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "chess/perft.h"
#include "chess/position.h"
#include "misc.h"

typedef struct {
    char *fen;
    unsigned int depth;
    unsigned int expected;
} PerftTest;

#define PERFT_TESTS 23
const PerftTest perft_tests[PERFT_TESTS] = {
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 1, 20},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 2, 400},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 3, 8902},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4, 197281},
    {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5, 4865609},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 1, 48},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 2, 2039},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 3, 97862},
    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 4, 4085603},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 1, 14},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 2, 191},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 3, 2812},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 4, 43238},
    {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 5, 674624},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 1, 6},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 2, 264},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 3, 9467},
    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4, 422333},
    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 1, 44},
    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 2, 1486},
    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 3, 62379},
    {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 4, 2103487},
    {"r6r/3bRk1p/1bpB1n2/3p3P/1p1P4/3B4/PKP2PP1/7R b - - 1 25", 1, 2},
};

void test_perft(void) {
    for (unsigned int i = 0; i < PERFT_TESTS; i++) {
        const PerftTest *test = &perft_tests[i];
        char *fen = test->fen;
        unsigned int depth = test->depth;
        unsigned int expected = test->expected;

        Position pos = import(fen);
        unsigned int actual = perft(&pos, depth);

        if (actual != expected) {
            printf("Perft Mismatch: %s| expected %u actual %u\n", fen, expected, actual);
            abort();
        }
    }
}

int main(int argc, char **argv) {
    Init();
    bool perft = false;
    if (argc == 1) {
        perft = true;
    } else if (strcasecmp(argv[1], "perft") == 0)
        perft = true;
    if (perft)
        test_perft();
    printf("OK\n");
    Fini();
    return 0;
}
