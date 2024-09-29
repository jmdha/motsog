#pragma once

#include "types.h"

#define EDGE     0xff818181818181ffllu
#define CENTER   0x7e7e7e7e7e7e00llu
#define CORNER_A 0x100000000000001llu
#define CORNER_H 0x8000000000000080llu
#define CORNERS  0x8100000000000081llu

#define RANK_1_BB 0xffllu
#define RANK_2_BB 0xff00llu
#define RANK_3_BB 0xff0000llu
#define RANK_4_BB 0xff000000llu
#define RANK_5_BB 0xff00000000llu
#define RANK_6_BB 0xff0000000000llu
#define RANK_7_BB 0xff000000000000llu
#define RANK_8_BB 0xff00000000000000llu

#define FILE_1_BB 0x101010101010101llu
#define FILE_2_BB 0x202020202020202llu
#define FILE_3_BB 0x404040404040404llu
#define FILE_4_BB 0x808080808080808llu
#define FILE_5_BB 0x1010101010101010llu
#define FILE_6_BB 0x2020202020202020llu
#define FILE_7_BB 0x4040404040404040llu
#define FILE_8_BB 0x8080808080808080llu

void init_masks(void);

BB ray(Square from, Square to);          // Has bits set between "from" and "to", inclusive
BB xray(Square from, Square to);         // Has bits set from "to" onwards, exclusive
BB ring(Square sq, int offset);          // Has bits set in a ring around "sq"
BB attacks_pawn(Square sq, Color color); // Denotes pawn attacks for an empty chess board
BB attacks_knight(Square sq);            // Denotes knight attacks for an empty chess board
BB attacks_bishop(Square sq);            // Denotes bishop attacks for an empty chess board
BB attacks_rook(Square sq);              // Denotes rook attacks for an empty chess board
BB attacks_king(Square sq);              // Denotes king attacks for an empty chess board
BB attacks(Square sq, Piece p);          // Simply a switch between individual attack functions
BB bab(Square sq, Piece p);              // Has bits set as "Attacks" excluding the edge
