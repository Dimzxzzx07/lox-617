#include "../include/lox617_hash.h"
#include <string.h>

static uint32_t rotl32(
    uint32_t v,
    uint32_t n
) {
    return
        (v << n) |
        (v >> (32 - n));
}

void lox617_blake3_seal(
    const uint8_t *data,
    uint32_t len,
    uint8_t *out
) {

    uint32_t state[8] = {
        0x6A09E667,
        0xBB67AE85,
        0x3C6EF372,
        0xA54FF53A,
        0x510E527F,
        0x9B05688C,
        0x1F83D9AB,
        0x5BE0CD19
    };

    for(uint32_t i = 0; i < len; i++) {

        uint32_t idx =
            i % 8;

        state[idx] ^=
            data[i];

        state[idx] =
            rotl32(
                state[idx],
                (i % 31) + 1
            );

        state[idx] +=
            0x9E3779B1U ^
            (i * 17);

        state[idx] ^=
            state[
                (idx + 3) % 8
            ];
    }

    for(int i = 0; i < 8; i++) {

        out[i * 4 + 0] =
            (state[i] >> 24) & 0xFF;

        out[i * 4 + 1] =
            (state[i] >> 16) & 0xFF;

        out[i * 4 + 2] =
            (state[i] >> 8) & 0xFF;

        out[i * 4 + 3] =
            state[i] & 0xFF;
    }
}