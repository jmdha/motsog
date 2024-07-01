#include "chess/attacks.h"
#include "chess/board.h"
#include "chess/perft.h"
#include "chess/zobrist.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

void Init() {
    InitAttacks();
    InitZobrist();
}

uint64_t DoPerft(char *pos, int depth) {
    Board board = ImportFEN(pos);
    return Perft(&board, depth);
}

#define perft_assert_eq(pos, depth, expected)                                                      \
    {                                                                                              \
        uint64_t actual = DoPerft(pos, depth);                                                     \
        cr_assert_eq(actual, expected, "depth %d actual %lu expected %lu", depth, actual,          \
                     (uint64_t)expected);                                                          \
    }

Test(PERFT, STARTPOS, .init = Init) {
    char *pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    perft_assert_eq(pos, 1, 20);
    perft_assert_eq(pos, 2, 400);
    perft_assert_eq(pos, 3, 8902);
    perft_assert_eq(pos, 4, 197281);
    perft_assert_eq(pos, 5, 4865609);
}

Test(PERFT, KIWIPETE, .init = Init) {
    char *pos = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
    perft_assert_eq(pos, 1, 48);
    perft_assert_eq(pos, 2, 2039);
    perft_assert_eq(pos, 3, 97862);
    perft_assert_eq(pos, 4, 4085603);
}

Test(PERFT, POS3, .init = Init) {
    char *pos = "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ";
    perft_assert_eq(pos, 1, 14);
    perft_assert_eq(pos, 2, 191);
    perft_assert_eq(pos, 3, 2812);
    perft_assert_eq(pos, 4, 43238);
    perft_assert_eq(pos, 5, 674624);
    perft_assert_eq(pos, 6, 11030083);
}

Test(PERFT, POS4, .init = Init) {
    char *pos = "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1";
    perft_assert_eq(pos, 1, 6);
    perft_assert_eq(pos, 2, 264);
    perft_assert_eq(pos, 3, 9467);
    perft_assert_eq(pos, 4, 422333);
    perft_assert_eq(pos, 5, 15833292);
}

Test(PERFT, POS5, .init = Init) {
    char *pos = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
    perft_assert_eq(pos, 1, 44);
    perft_assert_eq(pos, 2, 1486);
    perft_assert_eq(pos, 3, 62379);
    perft_assert_eq(pos, 4, 2103487);
    perft_assert_eq(pos, 5, 89941194);
}

Test(PERFT, POS6, .init = Init) {
    char *pos = "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10";
    perft_assert_eq(pos, 1, 46);
    perft_assert_eq(pos, 2, 2079);
    perft_assert_eq(pos, 3, 89890);
    perft_assert_eq(pos, 4, 3894594);
}
