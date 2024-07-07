#include "zobrist.h"
#include "types.h"

#define HASH_SEED 1070372

Hash ZOBRIST_TURN_KEY;
Hash ZOBRIST_CASTLE_KEYS[4];
Hash ZOBRIST_ENPASSANT_KEYS[SQUARE_COUNT];
Hash ZOBRIST_SQUARE_KEYS[PIECE_COUNT][SQUARE_COUNT];

Hash RandomNumber(void) {
    static Hash acc = HASH_SEED;

    acc ^= acc >> 12;
    acc ^= acc << 25;
    acc ^= acc >> 27;

    return acc * 2685821657736338717;
}

void InitZobrist(void) {
    for (int i = 0; i < PIECE_COUNT; i++)
        for (int t = 0; t < SQUARE_COUNT; t++)
            ZOBRIST_SQUARE_KEYS[i][t] = RandomNumber();

    for (int i = 0; i < 4; i++)
        ZOBRIST_CASTLE_KEYS[i] = RandomNumber();

    for (int i = 0; i < SQUARE_COUNT; i++)
        ZOBRIST_ENPASSANT_KEYS[i] = RandomNumber();

    ZOBRIST_TURN_KEY = RandomNumber();
}

Hash FlipSquare(Hash hash, Square square, PieceType piece_type) {
    return hash ^ ZOBRIST_SQUARE_KEYS[piece_type][square];
}

Hash FlipEnpassant(Hash hash, Square sq) { return hash ^ ZOBRIST_ENPASSANT_KEYS[sq]; }

Hash FlipCastle(Hash hash, Color color, Castling castle) {
    return hash ^ ZOBRIST_CASTLE_KEYS[2 * color + castle];
}

Hash FlipTurn(Hash hash) { return hash ^ ZOBRIST_TURN_KEY; }
