#include "eval.h"
#include "chess/bitboard.h"
#include "chess/position.h"
#include "values.h"

int Evaluate(const Position *pos, Color side) {
    int mg[2] = {0, 0};
    int eg[2] = {0, 0};
    unsigned int phase = 0;

    BB pieces = pos->colors[WHITE] | pos->colors[BLACK];
    while (pieces) {
        const Square sq = LSBPop(&pieces);
        const PieceType piece = GetPiece(pos, sq);
        const Color color = GetSquareColor(pos, sq);
        mg[color] += TABLE_MG[color][piece][sq];
        eg[color] += TABLE_EG[color][piece][sq];
        phase += PHASE[piece];
    }

    const unsigned int phase_mg = (phase > 24) ? 24 : phase;
    const unsigned int phase_eg = 24 - phase_mg;

    const int score_mg = mg[side] - mg[1 - side];
    const int score_eg = eg[side] - eg[1 - side];

    return (phase_mg * score_mg + phase_eg * score_eg);
}
