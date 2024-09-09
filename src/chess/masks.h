#pragma once

#include "types.h"

// masks.x
// Various bitmasks

void InitMasks(void);

BB Ray(Square from, Square to);         // Has bits set between "from" and "to", inclusive
BB XRay(Square from, Square to);        // Has bits set from "to" onwards, exclusive
BB Ring(Square sq, int offset);         // Has bits set in a ring around "sq"
BB PawnAttacks(Square sq, Color color); // Denotes pawn attacks for an empty chess board
BB KnightAttacks(Square sq);            // Denotes knight attacks for an empty chess board
BB BishopAttacks(Square sq);            // Denotes bishop attacks for an empty chess board
BB RookAttacks(Square sq);              // Denotes rook attacks for an empty chess board
BB KingAttacks(Square sq);              // Denotes king attacks for an empty chess board
BB Attacks(Square sq, PieceType p);     // Simply a switch between individual attack functions
BB BAB(Square sq, PieceType p);         // Has bits set as "Attacks" excluding the edge
