#include "eval.h"
#include "bit.h"
#include "values.h"

int Evaluate(const Position *pos, Color side) {
    const unsigned int phase_mg = (pos->phase > 24) ? 24 : pos->phase;
    const unsigned int phase_eg = 24 - phase_mg;

    const int score_mg = pos->eval_mg[side] - pos->eval_mg[1 - side];
    const int score_eg = pos->eval_eg[side] - pos->eval_eg[1 - side];

    return (phase_mg * score_mg + phase_eg * score_eg);
}
