#include <assert.h>

#include "types.h"
#include "zobrist.h"

#define HASH_SEED 1070372

Hash ZOBRIST_TURN_KEY;
Hash ZOBRIST_CASTLE_KEYS[4];
Hash ZOBRIST_ENPASSANT_KEYS[SQUARE_COUNT];
Hash ZOBRIST_SQUARE_KEYS[PIECE_COUNT][SQUARE_COUNT];

Hash ran_num(void) {
    static Hash acc = HASH_SEED;

    acc ^= acc >> 12;
    acc ^= acc << 25;
    acc ^= acc >> 27;

    return acc * 2685821657736338717;
}

void init_zobrist(void) {
    for (int i = 0; i < PIECE_COUNT; i++)
        for (int t = 0; t < SQUARE_COUNT; t++)
            ZOBRIST_SQUARE_KEYS[i][t] = ran_num();

    for (int i = 0; i < 4; i++)
        ZOBRIST_CASTLE_KEYS[i] = ran_num();

    for (int i = 0; i < SQUARE_COUNT; i++)
        ZOBRIST_ENPASSANT_KEYS[i] = ran_num();

    ZOBRIST_TURN_KEY = ran_num();
}

Hash flip_square(Hash hash, Square square, PieceType piece_type) {
    assert(square >= A1 && square <= H8);
    return hash ^ ZOBRIST_SQUARE_KEYS[piece_type][square];
}

Hash flip_enpassant(Hash hash, Square sq) {
    assert(sq >= A1 && sq <= H8);
    return hash ^ ZOBRIST_ENPASSANT_KEYS[sq];
}

Hash flip_castle(Hash hash, Color color, Castling castle) {
    assert(color == WHITE || color == BLACK);
    return hash ^ ZOBRIST_CASTLE_KEYS[2 * color + castle];
}

Hash flip_turn(Hash hash) { return hash ^ ZOBRIST_TURN_KEY; }
