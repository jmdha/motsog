#pragma once

#include "move.h"
#include "position.h"

#define MAX_BOARD_MOVES 2048

typedef struct Board {
    Position position_stack[MAX_BOARD_MOVES];
    uint64_t move_depth;
    uint64_t moves;
} Board;

Position *GetPosition(Board *board);
void ApplyMove(Board *board, Move move);
void UndoMove(Board *board, Move move);
Board ImportFEN(const char *fen);
void ImportMoves(Board *board, char *str);
Board DefaultBoard();
bool IsThreefold(Board *board);
