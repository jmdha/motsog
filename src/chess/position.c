#include <assert.h>
#include <ctype.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "bit.h"
#include "eval/values.h"
#include "masks.h"
#include "move.h"
#include "position.h"
#include "types.h"
#include "utility.h"
#include "zobrist.h"

Position position(void) {
    return import("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 ");
}

Position import(const char *fen) {
    Position pos = {.ep_square = SQUARE_NONE};
    pos.hash = calloc(MAX_PLY, sizeof(Hash));
    pos.hash++;
    for (int y = HEIGHT - 1; y >= 0; y--) {
        int remainder = WIDTH;
        while (remainder > 0) {
            if (isdigit(*fen))
                remainder -= *fen - 48;
            else {
                Piece type = CharToPieceType(*fen);
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
    return pos;
}

void position_free(Position *pos) {
    for (; *pos->hash; pos->hash--) {}
    free(pos->hash);
}

bool is_threefold(const Position *pos) {
    unsigned int count = 0;
    for (Hash *hash = pos->hash; *hash != 0; hash--)
        if (*pos->hash == *hash)
            count++;
    return count >= 3;
}

void apply(Position *out, const Position *pos, Move move) {
    memcpy(out, pos, sizeof(Position));
    out->hash++;
    *out->hash = *pos->hash;
    const Color us = out->turn;
    const Color nus = !us;
    const Square ori = MoveFrom(move);
    const Square dst = MoveTo(move);
    assert(GetPiece(out, dst) != KING);
    Piece piece = GetPiece(pos, ori);
    Piece target = PIECE_TYPE_NONE;
    Square ep = SQUARE_NONE;

    assert(popcount(out->pieces[KING]) == 2);
    assert(out->pieces[KING] & out->colors[WHITE]);
    assert(out->pieces[KING] & out->colors[BLACK]);

    RemovePiece(out, us, ori, piece);
    if (MoveIsPromotion(move))
        piece = MovePromotionPiece(move);

    if (MoveIsCapture(move)) {
        Square target_square = dst;
        if (MoveIsEnPassant(move))
            target_square = out->ep_square + (us == WHITE ? -8 : 8);
        target = GetPiece(pos, target_square);
        assert(target != KING);
        RemovePiece(out, nus, target_square, target);
    } else if (MoveIsDouble(move)) {
        ep = (us == WHITE) ? ori + 8 : ori - 8;
    } else if (MoveIsKingCastle(move)) {
        const Square rook_ori = (us == WHITE) ? H1 : H8;
        const Square rook_dst = (us == WHITE) ? F1 : F8;
        RemovePiece(out, us, rook_ori, ROOK);
        PlacePiece(out, us, rook_dst, ROOK);
    } else if (MoveIsQueenCastle(move)) {
        const Square rook_ori = (us == WHITE) ? A1 : A8;
        const Square rook_dst = (us == WHITE) ? D1 : D8;
        RemovePiece(out, us, rook_ori, ROOK);
        PlacePiece(out, us, rook_dst, ROOK);
    }

    assert(piece != KING || popcount(out->pieces[KING]) == 1);
    PlacePiece(out, us, dst, piece);

    // Handle castling stuff
    // TODO: Update zobrist
    //// Moving own pieces
    if (piece == KING)
        out->castling[us] = CASTLING_NONE;
    else if (piece == ROOK && sbb(ori) & CORNERS) {
        if (sbb(ori) & CORNER_A)
            out->castling[us] &= (~CASTLING_QUEEN);
        else if (sbb(ori) & CORNER_H)
            out->castling[us] &= (~CASTLING_KING);
    }

    //// Capturing their pieces
    if ((us == WHITE && dst == H8) || (us == BLACK && dst == H1))
        out->castling[nus] &= (~CASTLING_KING);
    else if ((us == WHITE && dst == A8) || (us == BLACK && dst == A1))
        out->castling[nus] &= (~CASTLING_QUEEN);

    out->ep_square = ep;
    out->turn = nus;
    assert(popcount(out->pieces[KING]) == 2);
    assert(out->pieces[KING] & out->colors[WHITE]);
    assert(out->pieces[KING] & out->colors[BLACK]);
}

void apply_moves(Position *pos, char *str) {
    for (char *p = strtok(str, " "); p != NULL; p = strtok(NULL, " ")) {
        if (strlen(p) != 4 && strlen(p) != 5)
            continue;
        Move move = ParseMove(pos, p);
        Position new_pos;
        apply(&new_pos, pos, move);
        *pos = new_pos;
    }
}

Color GetSquareColor(const Position *pos, Square sq) {
    BB b = (1ull << sq);
    if (b & pos->colors[WHITE])
        return WHITE;
    else if (b & pos->colors[BLACK])
        return BLACK;
    else
        return COLOR_NONE;
}

Piece GetPiece(const Position *pos, Square sq) {
    for (Piece p = PAWN; p <= KING; p++)
        if (pos->pieces[p] & sbb(sq))
            return p;
    return PIECE_TYPE_NONE;
}

void FlipPiece(Position *pos, Color color, Square sq, Piece type) {
    assert(color != COLOR_NONE);
    assert(sq != SQUARE_NONE);
    assert(type != PIECE_TYPE_NONE);
    pos->pieces[type] ^= sbb(sq);
    pos->colors[color] ^= sbb(sq);
    *pos->hash = flip_square(*pos->hash, sq, type);
    assert((pos->pieces[PAWN] | pos->pieces[KNIGHT] | pos->pieces[BISHOP] | pos->pieces[ROOK] |
            pos->pieces[QUEEN] | pos->pieces[KING]) == (pos->colors[WHITE] | pos->colors[BLACK]));
}

void PlacePiece(Position *pos, Color color, Square sq, Piece type) {
    FlipPiece(pos, color, sq, type);
    pos->eval_mg[color] += ValueMG(color, type, sq);
    pos->eval_eg[color] += ValueEG(color, type, sq);
    pos->phase += Phase(type);
}

void RemovePiece(Position *pos, Color color, Square sq, Piece type) {
    FlipPiece(pos, color, sq, type);
    pos->eval_mg[color] -= ValueMG(color, type, sq);
    pos->eval_eg[color] -= ValueEG(color, type, sq);
    pos->phase -= Phase(type);
}

void PrintPosition(Position *pos) {
    for (int y = HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x < WIDTH; x++) {
            Square sq = 8 * y + x;
            Piece p = GetPiece(pos, sq);
            Color c = GetSquareColor(pos, sq);
            switch (p) {
            case PAWN:
            case KNIGHT:
            case BISHOP:
            case ROOK:
            case QUEEN:
            case KING:
                printf("%c", PIECE_CHARS[c][p]);
                break;
            case PIECE_TYPE_NONE:
                printf(" ");
                break;
            }
        }
        printf("\n");
    }
}

bool IsKingSafe(const Position *pos, Color color) {
    assert(popcount(pos->pieces[KING]) == 2);
    Square king = lsb(pos->pieces[KING] & pos->colors[color]);

    BB occ = pos->colors[WHITE] | pos->colors[BLACK];
    BB pawns = pos->pieces[PAWN] & pos->colors[!color];
    BB knights = pos->pieces[KNIGHT] & pos->colors[!color];
    BB bishops = (pos->pieces[BISHOP] | pos->pieces[QUEEN]) & pos->colors[!color];
    BB rooks = (pos->pieces[ROOK] | pos->pieces[QUEEN]) & pos->colors[!color];

    // Checking jumpers
    if (attacks_pawn(king, color) & pawns)
        return false;
    if (attacks_knight(king) & knights)
        return false;

    // Checking sliders
    BB unblocked = attacks_rook(king) | attacks_bishop(king);
    BB potential_hazards = (attacks_rook(king) & rooks) | (attacks_bishop(king) & bishops);
    for (int offset = 1; potential_hazards && offset < 8; offset++) {
        BB potMoves = ring(king, offset) & unblocked;
        BB blockers = potMoves & occ;
        if (blockers & potential_hazards)
            return false;

        BB temp = blockers;
        while (temp) {
            BB bb = ~ray(king, lsbpop(&temp));
            unblocked &= bb;
            potential_hazards &= bb;
        }
    }

    return true;
}

BB GenerateAttackBoard(const Position *pos, Color color) {
    const BB occ = pos->colors[WHITE] | pos->colors[BLACK];
    BB result = 0;

    BB pawns = pos->pieces[PAWN] & pos->colors[color];
    while (pawns)
        result |= attacks_pawn(lsbpop(&pawns), color);

    BB knights = pos->pieces[KNIGHT] & pos->colors[color];
    while (knights)
        result |= attacks_knight(lsbpop(&knights));

    BB kings = pos->pieces[KING] & pos->colors[color];
    while (kings)
        result |= attacks_king(lsbpop(&kings));

    for (Piece p = BISHOP; p <= QUEEN; p++) {
        for (BB pieces = pos->pieces[p] & pos->colors[color]; pieces;) {
            const Square piece = lsbpop(&pieces);
            BB tmp = attacks(piece, p);

            for (BB b = occ & bab(piece, p); b != 0; b &= (b - 1)) {
                Square sq = lsb(b);
                tmp &= ~xray(piece, sq);
            }

            result |= tmp;
        }
    }

    return result;
}
