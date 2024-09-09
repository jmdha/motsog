#pragma once

#include "types.h"

void InitMasks(void);

BB Ray(Square from, Square to);
BB XRay(Square from, Square to);
BB Ring(Square sq, int offset);
BB BAB(Square sq, PieceType p);
BB PawnAttacks(Square sq, Color color);
BB KnightAttacks(Square sq);
BB BishopAttacks(Square sq);
BB RookAttacks(Square sq);
BB KingAttacks(Square sq);
BB Attacks(Square sq, PieceType p);
