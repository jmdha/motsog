#pragma once

#include <stdint.h>

#include "types.h"

enum MoveType {
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
};

void PrintMove(Move move);
Move ParseMove(Position *pos, char *str);

// clang-format off
#define MoveFrom(move)                       (Square)   (((move))        & 63)
#define MoveTo(move)                         (Square)   (((move)  >> 6)  & 63)
#define MoveGetType(move)                    (MoveType) (((move)  >> 12) & 15)
#define MoveIsCapture(move)                             (((move)  >> 12) & 4)
#define MoveIsPromotion(move)                           (((move)  >> 12) & 8)
#define MoveIsEnPassant(move)                           (((move)  >> 12) == EPCapture)
#define MoveIsDouble(move)                              (((move)  >> 12) == DoublePawnPush)
#define MoveIsKingCastle(move)                          (((move)  >> 12) == KingCastle)
#define MoveIsQueenCastle(move)                         (((move)  >> 12) == QueenCastle)
#define MoveIsCastle(move)                              (MoveIsKingCastle(move) || MoveIsQueenCastle(move))
#define MovePromotionPiece(move)             (Piece)    (((move)  >> 12) - 7 - (MoveIsCapture(move) ? 4 : 0))
#define MoveMake(from, to, move_type) (Move)     ((from) | ((to) << 6) | ((move_type) << 12))
// clang-format on
