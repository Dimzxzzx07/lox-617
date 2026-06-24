#include "../include/lox617_feistel.h"
#include "../include/lox617_sbox.h"
#include "../include/lox617_mds.h"
#include "../include/lox617_permutation.h"

static const uint8_t PRIMES[29] = {
    2,3,5,7,11,13,17,19,
    23,29,31,37,41,43,47,53,
    59,61,67,71,73,79,83,
    97,103,107,109,113,127
};

static inline uint64_t rotl64(
    uint64_t x,
    uint32_t s
) {
    s &= 63;

    return
        (x << s) |
        (x >> (64 - s));
}

static void round_function(
    uint8_t half[16],
    uint8_t *subkey,
    uint32_t round
) {

    uint8_t sbox[256];

    lox617_generate_sbox(
        sbox
    );

    for(int i = 0;
        i < 16;
        i++) {

        half[i] =
            sbox[
                (
                    half[i] ^
                    subkey[i]
                ) & 0xFF
            ];
    }

    for(int c = 0;
        c < 2;
        c++) {

        uint8_t chunk[8];

        for(int i = 0;
            i < 8;
            i++) {

            chunk[i] =
                half[
                    c * 8 + i
                ];
        }

        lox617_mds_apply(
            chunk
        );

        lox617_catmap(
            chunk
        );

        uint64_t val = 0;

        for(int i = 0;
            i < 8;
            i++) {

            val |=
                (
                    (
                        uint64_t)
                        chunk[i]
                    ) << (i * 8);
        }

        val =
            rotl64(
                val,
                PRIMES[
                    round % 29
                ]
            );

        for(int i = 0;
            i < 8;
            i++) {

            chunk[i] =
                (
                    val >>
                    (i * 8)
                ) & 0xFF;

            chunk[i] ^=
                subkey[
                    (
                        c * 8 + i
                    ) % 16
                ];
        }

        for(int i = 0;
            i < 8;
            i++) {

            half[
                c * 8 + i
            ] = chunk[i];
        }
    }
}

void lox617_encrypt_feistel(
    uint8_t *block,
    uint8_t *subkeys,
    uint32_t rounds
) {

    for(int i = 0;
        i < 32;
        i++) {

        block[i] ^=
            subkeys[i];
    }

    uint8_t left[16];
    uint8_t right[16];

    for(int i = 0;
        i < 16;
        i++) {

        left[i] =
            block[i];

        right[i] =
            block[16 + i];
    }

    for(uint32_t r = 0;
        r < rounds;
        r++) {

        uint8_t f[16];

        for(int i = 0;
            i < 16;
            i++) {

            f[i] =
                right[i];
        }

        round_function(
            f,
            &subkeys[
                r * 32
            ],
            r
        );

        for(int i = 0;
            i < 16;
            i++) {

            f[i] ^=
                left[i];
        }

        for(int i = 0;
            i < 16;
            i++) {

            left[i] =
                right[i];

            right[i] =
                f[i];
        }
    }

    for(int i = 0;
        i < 16;
        i++) {

        block[i] =
            left[i];

        block[16 + i] =
            right[i];
    }

    for(int i = 0;
        i < 32;
        i++) {

        block[i] ^=
            subkeys[
                (
                    rounds - 1
                ) * 32 + i
            ];
    }
}

void lox617_decrypt_feistel(
    uint8_t *block,
    uint8_t *subkeys,
    uint32_t rounds
) {

    for(int i = 0;
        i < 32;
        i++) {

        block[i] ^=
            subkeys[
                (
                    rounds - 1
                ) * 32 + i
            ];
    }

    uint8_t left[16];
    uint8_t right[16];

    for(int i = 0;
        i < 16;
        i++) {

        left[i] =
            block[i];

        right[i] =
            block[16 + i];
    }

    for(
        int r = rounds - 1;
        r >= 0;
        r--
    ) {

        uint8_t f[16];

        for(int i = 0;
            i < 16;
            i++) {

            f[i] =
                left[i];
        }

        round_function(
            f,
            &subkeys[
                r * 32
            ],
            r
        );

        for(int i = 0;
            i < 16;
            i++) {

            f[i] ^=
                right[i];
        }

        for(int i = 0;
            i < 16;
            i++) {

            right[i] =
                left[i];

            left[i] =
                f[i];
        }
    }

    for(int i = 0;
        i < 16;
        i++) {

        block[i] =
            left[i];

        block[16 + i] =
            right[i];
    }

    for(int i = 0;
        i < 32;
        i++) {

        block[i] ^=
            subkeys[i];
    }
}