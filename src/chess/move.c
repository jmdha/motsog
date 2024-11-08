#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chess/utility.h"
#include "move.h"
#include "position.h"
#include "types.h"

const char *PIECE_CHARS[COLOR_COUNT] = {"PNBRQK", "pnbrqk"};
const char *SQUARES[SQUARE_COUNT] = {
    "a1","b1","c1","d1","e1","f1","g1","h1",
    "a2","b2","c2","d2","e2","f2","g2","h2",
    "a3","b3","c3","d3","e3","f3","g3","h3",
    "a4","b4","c4","d4","e4","f4","g4","h4",
    "a5","b5","c5","d5","e5","f5","g5","h5",
    "a6","b6","c6","d6","e6","f6","g6","h6",
    "a7","b7","c7","d7","e7","f7","g7","h7",
    "a8","b8","c8","d8","e8","f8","g8","h8"
};

Square   move_from   (Move move) { return  move        & 63; }
Square   move_to     (Move move) { return (move >> 6)  & 63; }
MoveType move_type   (Move move) { return (move >> 12) & 15; }
bool     move_capture(Move move) { return (move >> 12) &  4; }
bool     move_promote(Move move) { return (move >> 12) &  8; }
Piece    move_piece  (Move move) { return (move >> 12) - 7 - 4 * move_capture(move); }

Move move_parse(Position *pos, char *str) {
    char from_column = str[0];
    char from_row = str[1];
    char to_column = str[2];
    char to_row = str[3];

    Square from = sq_from(sq_rankc(from_row), sq_filec(from_column));
    Square to = sq_from(sq_rankc(to_row), sq_filec(to_column));

    if (strlen(str) == 4) {
        Piece p = square_piece(pos, from);
        Piece c = square_piece(pos, to);
        if (p == KING && (strcmp(str, "e1g1") == 0 || strcmp(str, "e8g8") == 0))
            return move_make(from, to, KingCastle);
        else if (p == KING && (strcmp(str, "e1c1") == 0 || strcmp(str, "e8c8") == 0))
            return move_make(from, to, QueenCastle);
        else if (p == PAWN && (abs((int)from_row - (int)to_row)) > 1)
            return move_make(from, to, DoublePawnPush);
        else if (p == PAWN && (from_column != to_column)) {
            if (c == PIECE_TYPE_NONE)
                return move_make(from, to, EPCapture);
            else
                return move_make(from, to, Capture);
        } else if (c != PIECE_TYPE_NONE)
            return move_make(from, to, Capture);
        else
            return move_make(from, to, Quiet);

    } else {
        char prom_c = str[4];
        Piece promotion_piece = PIECE_TYPE_NONE;
        // clang-format off
        switch (prom_c) {
        case 'n': promotion_piece = KNIGHT; break;
        case 'b': promotion_piece = BISHOP; break;
        case 'r': promotion_piece = ROOK;   break;
        case 'q': promotion_piece = QUEEN;  break;
        }
        // clang-format on
        if (from_column != to_column)
            return move_make(from, to, NPromotionCapture + promotion_piece - KNIGHT);
        else
            return move_make(from, to, NPromotion + promotion_piece - KNIGHT);
    }
}

void move_print(Move move) {
    printf("%s%s", SQUARES[move_from(move)], SQUARES[move_to(move)]);
    if (move_promote(move))
        printf("%c", PIECE_CHARS[BLACK][move_piece(move)]);
}

Move move_make(Square ori, Square dst, MoveType type) {
    return ori | (dst << 6) | (type << 12);
}

