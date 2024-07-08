#include "move.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void PrintMove(Move move) {
    Square from = MoveFrom(move);
    Square to = MoveTo(move);
    printf("%s%s", SQUARES[from], SQUARES[to]);
    if (MoveIsPromotion(move))
        printf("%c", PIECE_CHARS[BLACK][MovePromotionPiece(move)]);
}

Move ParseMove(Position *pos, char *str) {
    char from_column = str[0];
    char from_row = str[1];
    char to_column = str[2];
    char to_row = str[3];

    Square from = SquareFromChar(from_column, from_row);
    Square to = SquareFromChar(to_column, to_row);

    if (strlen(str) == 4) {
        PieceType p = GetPiece(pos, from);
        PieceType c = GetPiece(pos, to);
        if (p == KING && (strcmp(str, "e1g1") == 0 || strcmp(str, "e8g8") == 0))
            return MoveMake(from, to, KingCastle);
        else if (p == KING && (strcmp(str, "e1c1") == 0 || strcmp(str, "e8c8") == 0))
            return MoveMake(from, to, QueenCastle);
        else if (p == PAWN && (abs((int)from_row - (int)to_row)) > 1)
            return MoveMake(from, to, DoublePawnPush);
        else if (p == PAWN && (from_column != to_column)) {
            if (c == PIECE_TYPE_NONE)
                return MoveMake(from, to, EPCapture);
            else
                return MoveMake(from, to, Capture);
        } else if (c != PIECE_TYPE_NONE)
            return MoveMake(from, to, Capture);
        else
            return MoveMake(from, to, Quiet);

    } else {
        char prom_c = str[4];
        PieceType promotion_piece = PIECE_TYPE_NONE;
        // clang-format off
        switch (prom_c) {
        case 'n': promotion_piece = KNIGHT; break;
        case 'b': promotion_piece = BISHOP; break;
        case 'r': promotion_piece = ROOK;   break;
        case 'q': promotion_piece = QUEEN;  break;
        }
        // clang-format on
        if (from_column != to_column)
            return MoveMake(from, to, NPromotionCapture + promotion_piece - KNIGHT);
        else
            return MoveMake(from, to, NPromotion + promotion_piece - KNIGHT);
    }
}
