#ifndef LOX617_SBOX_H
#define LOX617_SBOX_H

#include <stdint.h>

extern uint8_t lox_sbox[256];

void lox617_generate_sbox(
    uint8_t sbox[256]
);

uint8_t lox_gf256_mul(
    uint8_t a,
    uint8_t b
);

#endif