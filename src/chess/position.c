#include "position.h"
#include "bit.h"
#include "masks.h"
#include "types.h"
#include "zobrist.h"
#include <assert.h>
#include <memory.h>
#include <stdio.h>

void ClearPos(Position *pos) {
    memset(pos, 0, sizeof(Position));
    pos->ep_square = SQUARE_NONE;
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

PieceType GetPiece(const Position *pos, Square sq) {
    for (PieceType p = PAWN; p <= KING; p++)
        if (pos->pieces[p] & ToBB(sq))
            return p;
    return PIECE_TYPE_NONE;
}

void FlipPiece(Position *pos, Color color, Square sq, PieceType type) {
    assert(color != COLOR_NONE);
    assert(sq != SQUARE_NONE);
    assert(type != PIECE_TYPE_NONE);
    pos->pieces[type] ^= ToBB(sq);
    pos->colors[color] ^= ToBB(sq);
    pos->hash = flip_square(pos->hash, sq, type);
    assert((pos->pieces[PAWN] | pos->pieces[KNIGHT] | pos->pieces[BISHOP] | pos->pieces[ROOK] |
            pos->pieces[QUEEN] | pos->pieces[KING]) == (pos->colors[WHITE] | pos->colors[BLACK]));
}

void PlacePiece(Position *pos, Color color, Square sq, PieceType type) {
    FlipPiece(pos, color, sq, type);
}

void RemovePiece(Position *pos, Color color, Square sq, PieceType type) {
    FlipPiece(pos, color, sq, type);
}

void PrintPosition(Position *pos) {
    for (int y = HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x < WIDTH; x++) {
            Square sq = 8 * y + x;
            PieceType p = GetPiece(pos, sq);
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
    if (PawnAttacks(king, color) & pawns)
        return false;
    if (KnightAttacks(king) & knights)
        return false;

    // Checking sliders
    BB unblocked = RookAttacks(king) | BishopAttacks(king);
    BB potential_hazards = (RookAttacks(king) & rooks) | (BishopAttacks(king) & bishops);
    for (int offset = 1; potential_hazards && offset < 8; offset++) {
        BB ring = Ring(king, offset);
        BB potMoves = ring & unblocked;
        BB blockers = potMoves & occ;
        if (blockers & potential_hazards)
            return false;

        BB temp = blockers;
        while (temp) {
            BB ray = ~Ray(king, lsbpop(&temp));
            unblocked &= ray;
            potential_hazards &= ray;
        }
    }

    return true;
}

BB GenerateAttackBoard(const Position *pos, Color color) {
    const BB occ = pos->colors[WHITE] | pos->colors[BLACK];
    BB attacks = 0;

    BB pawns = pos->pieces[PAWN] & pos->colors[color];
    while (pawns)
        attacks |= PawnAttacks(lsbpop(&pawns), color);

    BB knights = pos->pieces[KNIGHT] & pos->colors[color];
    while (knights)
        attacks |= KnightAttacks(lsbpop(&knights));

    BB kings = pos->pieces[KING] & pos->colors[color];
    while (kings)
        attacks |= KingAttacks(lsbpop(&kings));

    for (PieceType p = BISHOP; p <= QUEEN; p++) {
        for (BB pieces = pos->pieces[p] & pos->colors[color]; pieces;) {
            const Square piece = lsbpop(&pieces);
            BB attacks1 = Attacks(piece, p);

            for (BB b = occ & BAB(piece, p); b != 0; b &= (b - 1)) {
                Square sq = lsb(b);
                attacks1 &= ~XRay(piece, sq);
            }

            attacks |= attacks1;
        }
    }

    return attacks;
}
