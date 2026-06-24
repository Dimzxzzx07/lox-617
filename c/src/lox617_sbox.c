#include "../include/lox617_sbox.h"

#include <stdint.h>

uint8_t lox_sbox[256];

static uint8_t gf_mul(
    uint8_t a,
    uint8_t b
) {

    uint8_t p = 0;

    for(int i = 0;
        i < 8;
        i++) {

        if(b & 1)
            p ^= a;

        uint8_t hi =
            a & 0x80;

        a <<= 1;

        if(hi)
            a ^= 0x1B;

        b >>= 1;
    }

    return p;
}

static uint8_t gf_inv(
    uint8_t a
) {

    if(a == 0)
        return 0;

    uint8_t t = 1;

    for(int i = 0;
        i < 254;
        i++) {

        t =
            gf_mul(
                t,
                a
            );
    }

    return t;
}

static inline uint8_t rotl8(
    uint8_t x,
    uint8_t r
) {

    r &= 7;

    if(r == 0)
        return x;

    return
        (uint8_t)(
            (x << r) |
            (x >> (8 - r))
        );
}

void lox617_generate_sbox(
    uint8_t sbox[256]
) {

    for(int i = 0;
        i < 256;
        i++) {

        uint8_t x =
            gf_inv(
                (uint8_t)i
            );

        uint8_t y =
            x ^
            rotl8(x, 1) ^
            rotl8(x, 2) ^
            0x63;

        sbox[i] = y;
    }
}