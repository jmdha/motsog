# motsog
<img src="https://img.shields.io/github/license/jmdha/motsog"> <img src="https://img.shields.io/github/last-commit/jmdha/motsog"> <img src="https://img.shields.io/github/commit-activity/m/jmdha/motsog"> 

[![lichess-bullet](https://lichess-shield.vercel.app/api?username=motsog&format=bullet)](https://lichess.org/@/motsog/perf/bullet)
[![lichess-blitz](https://lichess-shield.vercel.app/api?username=motsog&format=blitz)](https://lichess.org/@/motsog/perf/blitz)

## Building
This is a C program without any dependencies, as such, only a C compiler and Make is required.
```
git clone https://github.com/jmdha/motsog.git && \
cd motsog && \
make
```
You will then find the executable "motsog" in the base directory. It follows the UCI protocol for usage.

## Engine Structure
The engine is split into three parts: 

  - Chess: Loading of a chess position, maintaining state, move generation for a given board position, etc.
  - Evaluation: Giving a numeric score of a given state based on set values.
  - Search: Exploring the search tree for the best possible sequence of moves from a given position.

A more technical description is given in the following sections.

## Chess
### State
The engine is based on bitboards which is a unsigned 64 bit number where each bit represents a square. As such, piece positions on the board are stored as a 8 tuple of bitboards. Where 2 bitboards are for colors, i.e. 1 for white and 1 for black, and 6 bitboards for pieces, 1 for each piece respectively. Thus, retrieving pieces of a certain type and color is a simple bitwise operation, namely bitwise AND, between the bitboard of the color, and piece.

### Move
A move is stored as an unsigned 16 bit integer, which contains three pieces of information.
The first 12 bits are used to store the from and to squares of the move, i.e. from square index "a" to square index "b". Then 4 bits are used for flags such as "is king castle" or "is capture".

### Move Generation
This engine follows the philosophy of pseudo legal move generation, where the issue of "king safety" is ignored. Instead, each move is checked in search for whether it is legal.

There exists two types of pieces for the purposes of move generation: jumpers and sliders. A jumper - pawn(except double push), knight and king - ignores pieces inbetween its current square and target square. Whereas a slider - pawn(double push), bishop, rook and queen - needs to consider blockers.

Generating moves for jumpers is trivial in a bitboard engine, as one can simply create a bitboard for the possible moves for each square. Say that you want the possible moves for a knight on D4, then on can simply do a bitwise and between the possible moves for the square and a bitboard of the unoccupied squares.

## Evaluation
This engine considers material and positional value. Where material value is a static value associated with each piece, with the traditional values being 100, 300, 300, 500, 900 for the pawn, knight, bishop, rook, and queen respectively. The material evaluation of a given position is then the difference in count of all pieces times their respective value.

However, the positional value is tad more dynamic. It associates a value for each piece on each square, and thus requires a bit more computation to calculate. Which is why this engine calculates the standings in an incremental manner, where both the material and position value is updated whenever a piece is moved.

## Search
TBD.
