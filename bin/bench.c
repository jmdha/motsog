#include <stdio.h>
#include <time.h>

#include "chess/perft.h"
#include "chess/position.h"
#include "misc.h"

long long int total_nodes = 0;
double total_time = 0;

float time_perft(const char *fen, unsigned int depth) {
    Position pos = import(fen);
    clock_t t0 = clock();
    perft(&pos, depth);
    clock_t t1 = clock();
    return (float)(t1 - t0) / CLOCKS_PER_SEC;
}

void bench_perft(const char *fen, unsigned int depth, long long int nodes) {
    const float m_nodes = nodes / 1000000.0f;
    float total = 0;
    for (unsigned int i = 0; i < 3; i++) {
        float time = time_perft(fen, depth);
        total += time;
        total_time += time;
        total_nodes += nodes;
        printf("%f s (%.0f M n/s) %s %u\n", time, m_nodes / time, fen, depth);
    }
    printf("avg: %f (%.0f M n/s)\n", total / 3, 3 * m_nodes / total);
}

int main(void) {
    Init();
    printf("warming up...\n");
    time_perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6);
    printf("beginning benchmark\n");
    bench_perft("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 6, 119060324);
    bench_perft("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 5, 193690690);
    bench_perft("8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 6, 11030083);
    bench_perft("r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 6, 706045033);
    bench_perft("rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ", 5, 89941194);
    const float m_nodes = total_nodes / 1000000.0f;
    printf("aggregate: %lld n %f s (%.0f M n/s)\n", total_nodes, total_time, m_nodes / total_time);
    Fini();
}
