#include "chess/attacks.h"
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

#define bb_assert_eq(exp1, exp2) cr_expect_eq(exp1, exp2, "actual %lu expected %lu", exp1, exp2)

Test(ATTACKS, RING, .init = InitAttacks) {
    bb_assert_eq(Ring(A1, 7), 0xff80808080808080);
    bb_assert_eq(Ring(A8, 7), 0x80808080808080ff);
    bb_assert_eq(Ring(H1, 7), 0xff01010101010101);
    bb_assert_eq(Ring(H8, 7), 0x1010101010101ff);
}

Test(ATTACKS, RAY, .init = InitAttacks) {
    bb_assert_eq(Ray(A1, A2), 0x101010101010100);
    bb_assert_eq(Ray(A1, B1), 0xfelu);
    bb_assert_eq(Ray(A1, B2), 0x8040201008040200);

    bb_assert_eq(Ray(A8, A7), 0x1010101010101);
    bb_assert_eq(Ray(A8, B8), 0xfe00000000000000);
    bb_assert_eq(Ray(A8, B7), 0x2040810204080);

    bb_assert_eq(Ray(H1, H2), 0x8080808080808000);
    bb_assert_eq(Ray(H1, G1), 0x7flu);
    bb_assert_eq(Ray(H1, G2), 0x102040810204000);

    bb_assert_eq(Ray(H8, H7), 0x80808080808080);
    bb_assert_eq(Ray(H8, G8), 0x7f00000000000000);
    bb_assert_eq(Ray(H8, G7), 0x40201008040201);
}

Test(ATTACKS, XRAY, .init = InitAttacks) {
    bb_assert_eq(XRay(A1, A2), 0x101010101010000);
    bb_assert_eq(XRay(A1, B1), 0xfclu);
    bb_assert_eq(XRay(A1, B2), 0x8040201008040000);

    bb_assert_eq(XRay(A8, A7), 0x10101010101);
    bb_assert_eq(XRay(A8, B8), 0xfc00000000000000);
    bb_assert_eq(XRay(A8, B7), 0x40810204080);

    bb_assert_eq(XRay(H1, H2), 0x8080808080800000);
    bb_assert_eq(XRay(H1, G1), 0x3flu);
    bb_assert_eq(XRay(H1, G2), 0x102040810200000);

    bb_assert_eq(XRay(H8, H7), 0x808080808080);
    bb_assert_eq(XRay(H8, G8), 0x3f00000000000000);
    bb_assert_eq(XRay(H8, G7), 0x201008040201);
}

Test(ATTACKS, BAB, .init = InitAttacks) {
    bb_assert_eq(BAB(A1, BISHOP), 0x40201008040200);
    bb_assert_eq(BAB(A8, BISHOP), 0x2040810204000);
    bb_assert_eq(BAB(H1, BISHOP), 0x2040810204000);
    bb_assert_eq(BAB(H8, BISHOP), 0x40201008040200);

    bb_assert_eq(BAB(A1, ROOK), 0x101010101017e);
    bb_assert_eq(BAB(A8, ROOK), 0x7e01010101010100);
    bb_assert_eq(BAB(H1, ROOK), 0x8080808080807e);
    bb_assert_eq(BAB(H8, ROOK), 0x7e80808080808000);

    bb_assert_eq(BAB(A1, QUEEN), 0x4121110905037e);
    bb_assert_eq(BAB(A8, QUEEN), 0x7e03050911214100);
    bb_assert_eq(BAB(H1, QUEEN), 0x82848890a0c07e);
    bb_assert_eq(BAB(H8, QUEEN), 0x7ec0a09088848200);
}

Test(ATTACKS, PSEUDO, .init = InitAttacks) {
    bb_assert_eq(KnightAttacks(A1), 0x20400lu);
    bb_assert_eq(KnightAttacks(A8), 0x4020000000000);
    bb_assert_eq(KnightAttacks(H1), 0x402000lu);
    bb_assert_eq(KnightAttacks(H8), 0x20400000000000);

    bb_assert_eq(BishopAttacks(A1), 0x8040201008040200);
    bb_assert_eq(BishopAttacks(A8), 0x2040810204080);
    bb_assert_eq(BishopAttacks(H1), 0x102040810204000);
    bb_assert_eq(BishopAttacks(H8), 0x40201008040201);

    bb_assert_eq(RookAttacks(A1), 0x1010101010101fe);
    bb_assert_eq(RookAttacks(A8), 0xfe01010101010101);
    bb_assert_eq(RookAttacks(H1), 0x808080808080807f);
    bb_assert_eq(RookAttacks(H8), 0x7f80808080808080);

    bb_assert_eq(KingAttacks(A1), 0x302lu);
    bb_assert_eq(KingAttacks(A8), 0x203000000000000);
    bb_assert_eq(KingAttacks(H1), 0xc040lu);
    bb_assert_eq(KingAttacks(H8), 0x40c0000000000000);
}
