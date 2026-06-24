#include "../include/lox617_mds.h"

static uint8_t mds_circulant[8] = {
    0x17, 0x31, 0x59, 0x83,
    0xA7, 0xC1, 0xD3, 0xEF
};

uint8_t lox_gf256_mul(uint8_t a, uint8_t b) {
    uint8_t p = 0;

    for(int i = 0; i < 8; i++) {
        if(b & 1) {
            p ^= a;
        }

        uint8_t hi = a & 0x80;

        a <<= 1;

        if(hi) {
            a ^= 0x1B;
        }

        b >>= 1;
    }

    return p;
}

void lox617_mds_apply(uint8_t *block) {
    uint8_t temp[8] = {0};

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            temp[i] ^= lox_gf256_mul(
                block[j],
                mds_circulant[(i + j) % 8]
            );
        }
    }

    for(int i = 0; i < 8; i++) {
        block[i] = temp[i];
    }
}