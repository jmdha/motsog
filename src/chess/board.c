#include "board.h"
#include "bit.h"
#include "move.h"
#include "position.h"
#include "types.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include "masks.h"

Position *GetPosition(Board *board) { return &board->position_stack[board->move_depth]; }

bool IsThreefold(Board *board) {
    Hash hash = GetPosition(board)->hash;
    for (int i = 0; i < (int)board->move_depth - 1; i++)
        if (hash == board->position_stack[i].hash)
            return true;
    return false;
}

void ApplyMove(Board *board, Move move) {
    assert(popcount(GetPosition(board)->pieces[KING]) == 2);
    assert(move != 0);
    Position *pos = &board->position_stack[board->move_depth + 1];
    memcpy(pos, &board->position_stack[board->move_depth],
           sizeof(Position) - sizeof(Color) - sizeof(Square));

    const Color us = GetPosition(board)->turn;
    const Color nus = !us;
    const Square ori = MoveFrom(move);
    const Square dst = MoveTo(move);
    assert(GetPiece(pos, dst) != KING);
    PieceType piece = GetPiece(pos, ori);
    PieceType target = PIECE_TYPE_NONE;
    Square ep = SQUARE_NONE;

    assert(popcount(pos->pieces[KING]) == 2);
    assert(pos->pieces[KING] & pos->colors[WHITE]);
    assert(pos->pieces[KING] & pos->colors[BLACK]);

    RemovePiece(pos, us, ori, piece);
    if (MoveIsPromotion(move))
        piece = MovePromotionPiece(move);

    if (MoveIsCapture(move)) {
        Square target_square = dst;
        if (MoveIsEnPassant(move))
            target_square = GetPosition(board)->ep_square + (us == WHITE ? -8 : 8);
        target = GetPiece(pos, target_square);
        assert(target != KING);
        RemovePiece(pos, nus, target_square, target);
    } else if (MoveIsDouble(move)) {
        ep = (us == WHITE) ? ori + 8 : ori - 8;
    } else if (MoveIsKingCastle(move)) {
        const Square rook_ori = (us == WHITE) ? H1 : H8;
        const Square rook_dst = (us == WHITE) ? F1 : F8;
        RemovePiece(pos, us, rook_ori, ROOK);
        PlacePiece(pos, us, rook_dst, ROOK);
    } else if (MoveIsQueenCastle(move)) {
        const Square rook_ori = (us == WHITE) ? A1 : A8;
        const Square rook_dst = (us == WHITE) ? D1 : D8;
        RemovePiece(pos, us, rook_ori, ROOK);
        PlacePiece(pos, us, rook_dst, ROOK);
    }

    assert(piece != KING || popcount(pos->pieces[KING]) == 1);
    PlacePiece(pos, us, dst, piece);

    // Handle castling stuff
    // TODO: Update zobrist
    //// Moving own pieces
    if (piece == KING)
        pos->castling[us] = CASTLING_NONE;
    else if (piece == ROOK && ToBB(ori) & CORNERS) {
        if (ToBB(ori) & CORNER_A)
            pos->castling[us] &= (~CASTLING_QUEEN);
        else if (ToBB(ori) & CORNER_H)
            pos->castling[us] &= (~CASTLING_KING);
    }

    //// Capturing their pieces
    if (dst == KING_CORNER[nus])
        pos->castling[nus] &= (~CASTLING_KING);
    else if (dst == QUEEN_CORNER[nus])
        pos->castling[nus] &= (~CASTLING_QUEEN);

    pos->ep_square = ep;
    pos->turn = nus;
    board->move_depth++;
    board->moves++;
    assert(popcount(pos->pieces[KING]) == 2);
    assert(pos->pieces[KING] & pos->colors[WHITE]);
    assert(pos->pieces[KING] & pos->colors[BLACK]);
}

void UndoMove(Board *board, Move) { board->move_depth--; }

Board DefaultBoard(void) {
    return ImportFEN("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ");
}

Board ImportFEN(const char *fen) {
    Position pos;
    ClearPos(&pos);

    for (int y = HEIGHT - 1; y >= 0; y--) {
        int remainder = WIDTH;
        while (remainder > 0) {
            if (isdigit(*fen))
                remainder -= *fen - 48;
            else {
                PieceType type = CharToPieceType(*fen);
                Color color = islower(*fen) ? BLACK : WHITE;
                Square sq = 8 * y + WIDTH - remainder--;
                PlacePiece(&pos, color, sq, type);
            }
            fen++;
        }
        fen++;
    }

    pos.turn = (tolower(*fen++) == 'w') ? WHITE : BLACK;

    fen++;

    while ((*fen) != ' ' && strlen(fen) > 0) {
        switch (*fen) {
        case 'K':
        case 'k':
            pos.castling[!!islower(*fen)] |= CASTLING_KING;
            break;
        case 'Q':
        case 'q':
            pos.castling[!!islower(*fen)] |= CASTLING_QUEEN;
            break;
        default:
            break;
        }
        fen++;
    }

    Board board;
    board.move_depth = 0;
    board.position_stack[0] = pos;
    board.moves = 0;
    return board;
}

void ExportFEN(Board *board) {
    const Position *pos = GetPosition(board);

    for (int y = 0; y < 8; y++) {
        unsigned int pieces = 0;
        for (unsigned int x = 0; x < 8; x++) {
            const Square sq = 8 * (7 - y) + x;
            const PieceType piece = GetPiece(pos, sq);
            if (piece == PIECE_TYPE_NONE)
                continue;
            if (pieces < x)
                printf("%d", x - pieces);

            const Color color = GetSquareColor(pos, sq);
            printf("%c", PIECE_CHARS[color][piece]);
            pieces = x + 1;
        }
        if (pieces < 8)
            printf("%d", 8 - pieces);
        if (y != 7)
            printf("/");
    }

    printf("\n");
}

void ImportMoves(Board *board, char *str) {
    for (char *p = strtok(str, " "); p != NULL; p = strtok(NULL, " ")) {
        if (strlen(p) != 4 && strlen(p) != 5)
            continue;
        Move move = ParseMove(GetPosition(board), p);
        ApplyMove(board, move);
    }
}
