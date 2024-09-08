#include "eval.h"
#include "chess/bitboard.h"
#include "chess/position.h"
#include "values.h"

int Evaluate(const Position *pos, Color side) {
    int mg[2] = {0, 0};
    int eg[2] = {0, 0};
    unsigned int phase = 0;

    for (unsigned int i = 0; i < COLOR_COUNT; i++) {
        for (unsigned int t = 0; t < PIECE_COUNT; t++) {
            BB pieces = pos->colors[i] & pos->pieces[t];
            while (pieces) {
                const Square sq = LSBPop(&pieces);
                mg[i] += TABLE_MG[i][t][sq];
                eg[i] += TABLE_EG[i][t][sq];
                phase += PHASE[t];
            }
        }
    }

    const unsigned int phase_mg = (phase > 24) ? 24 : phase;
    const unsigned int phase_eg = 24 - phase_mg;

    const int score_mg = mg[side] - mg[1 - side];
    const int score_eg = eg[side] - eg[1 - side];

    return (phase_mg * score_mg + phase_eg * score_eg);
}
