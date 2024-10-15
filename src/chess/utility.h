#pragma once

#include "types.h"

BB sbb(Square sq);                // A bitboard with the "sq" set
BB shift_up(Color color, BB b);   // Shifts board up according to color
BB shift_down(Color color, BB b); // Shifts board down according to color

unsigned int dist_horizontal(Square a, Square b); // Returns horizontal distance between squares
unsigned int dist_vertical(Square a, Square b);   // Returns vertical distance between squares
unsigned int dist_min(Square a, Square b);        // Returns min of horizontal and vertical distance
unsigned int dist_max(Square a, Square b);        // Returns max of horizontal and vertical distance
unsigned int dist_manhattan(Square a, Square b);  // Returns Manhattan distance between squares

File sq_file(Square sq);              // Returns the file of the square
File sq_filec(char sq);               // Returns the file associated with character (a-h)
Rank sq_rank(Square sq);              // Returns the rank of the square
Rank sq_rankc(char sq);               // Returns the rank associated with character (1-8)
Square sq_from(Rank rank, File file); // Returns the square where rank and file intersects
Piece piece_from(char c);             // Returns the piece that corresponds to the character
