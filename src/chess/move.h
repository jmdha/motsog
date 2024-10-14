#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "types.h"

typedef enum MoveType {
    Quiet = 0,
    DoublePawnPush = 1,
    KingCastle = 2,
    QueenCastle = 3,
    Capture = 4,
    EPCapture = 5,
    NPromotion = 8,
    BPromotion = 9,
    RPromotion = 10,
    QPromotion = 11,
    NPromotionCapture = 12,
    BPromotionCapture = 13,
    RPromotionCapture = 14,
    QPromotionCapture = 15
} MoveType;

Move     move_make   (Square ori, Square dst, MoveType type);
Move     move_parse  (Position *pos, char *str);
void     move_print  (Move move);
Square   move_from   (Move move);
Square   move_to     (Move move);
MoveType move_type   (Move move);
bool     move_capture(Move move);
bool     move_promote(Move move);
Piece    move_piece  (Move move);
