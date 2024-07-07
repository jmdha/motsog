#include "eval.h"
#include "chess/bitboard.h"
#include "values.h"

static int EvaluateMaterial(const Position *pos, Color side) {
    int val = 0;

    for (PieceType p = PAWN; p < KING; p++) {
        const unsigned int count = Popcount(pos->pieces[p] & pos->colors[side]);
        val += count * MATERIAL[p];
    }

    return val;
}

static int EvaluatePosition(const Position *pos, Color side) {
    int val = 0;

    for (PieceType p = PAWN; p < KING; p++) {
        BB pieces = pos->pieces[p] & pos->colors[side];
        while (pieces) {
            if (side == WHITE)
                val += POSITION[p][LSBPop(&pieces) ^ 56];
            else
                val += POSITION[p][LSBPop(&pieces)];
        }
    }

    return val;
}

int Evaluate(const Position *pos, Color side) {
    return EvaluateMaterial(pos, side) - EvaluateMaterial(pos, !side) +
           EvaluatePosition(pos, side) - EvaluatePosition(pos, !side);
}
