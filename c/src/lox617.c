#include "../include/lox617.h"
#include "../include/lox617_core.h"
#include "../include/lox617_sbox.h"
#include "../include/lox617_mds.h"
#include "../include/lox617_nlfsr.h"
#include "../include/lox617_permutation.h"
#include "../include/lox617_keyschedule.h"

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>

static const uint32_t PRIMES[29] = {
    2,3,5,7,11,13,17,19,
    23,29,31,37,41,43,
    47,53,59,61,67,71,
    73,79,83,97,103,
    107,109,113,127
};

static inline uint64_t rotl64(
    uint64_t x,
    uint32_t r
) {

    r &= 63;

    if(r == 0)
        return x;

    return
        (x << r) |
        (x >> (64 - r));
}

void lox617_encrypt(
    uint8_t *block,
    const uint8_t *key,
    size_t rounds
) {

    uint8_t subkeys[32 * 32];

    uint64_t state =
        0x617617617617617ULL;

    lox617_generate_sbox(
        lox_sbox
    );

    lox617_expand_key(
        key,
        subkeys,
        rounds
    );

    for(size_t r = 0;
        r < rounds;
        r++) {

        for(int i = 0;
            i < 32;
            i++) {

            block[i] =
                lox_sbox[
                    (uint8_t)block[i]
                ];
        }

        for(int i = 0;
            i < 4;
            i++) {

            uint64_t val;

            memcpy(
                &val,
                &block[i * 8],
                8
            );

            uint32_t shift =
                PRIMES[r % 29] % 64;

            val =
                rotl64(
                    val,
                    shift
                );

            memcpy(
                &block[i * 8],
                &val,
                8
            );
        }

        for(int i = 0;
            i < 4;
            i++) {

            lox617_mds_apply(
                &block[i * 8]
            );
        }

        for(int i = 0;
            i < 32;
            i++) {

            block[i] ^=
                subkeys[
                    r * 32 + i
                ];
        }

        lox617_nlfsr_clock(
            &state
        );

        for(int i = 0;
            i < 32;
            i++) {

            block[i] ^=
                (uint8_t)(
                    state >>
                    (i % 8)
                );
        }
        
        for(int i = 0;
            i < 4;
            i++) {

            lox617_catmap(
                &block[i * 8]
            );
        }

        printf(
            "ROUND %zu\n",
            r
        );

        for(int i = 0;
            i < 32;
            i++) {

            printf(
                "%02X",
                block[i]
            );
        }

        printf("\n\n");
    }
}