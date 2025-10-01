#include "eval.h"
#include "misc.h"

int16_t eval(const Position *pos, Color side) {
    const unsigned int phase_mg = min(24u, pos->phase);
    const unsigned int phase_eg = 24 - phase_mg;

    const int16_t score_mg = pos->eval_mg[side] - pos->eval_mg[1 - side];
    const int16_t score_eg = pos->eval_eg[side] - pos->eval_eg[1 - side];

    return (phase_mg * score_mg + phase_eg * score_eg);
}
