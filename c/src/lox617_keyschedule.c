#include "../include/lox617_keyschedule.h"
#include "../include/lox617_sbox.h"
#include "../include/lox617_mds.h"

#include <stdint.h>
#include <stdio.h>

static const uint8_t PRIMES[29] = {
    2,3,5,7,11,13,17,19,
    23,29,31,37,41,43,47,53,
    59,61,67,71,73,79,83,
    97,103,107,109,113,127
};

static inline uint8_t rotl8(
    uint8_t x,
    uint8_t s
) {

    s &= 7;

    if(s == 0)
        return x;

    return (uint8_t)(
        (
            (uint8_t)(x << s)
        ) |
        (
            (uint8_t)(x >> (8 - s))
        )
    );
}

static void cubic_hardening(
    const uint8_t input[32],
    uint8_t output[32]
) {

    for(int i = 0; i < 32; i++) {

        uint32_t c =
            (uint32_t)input[i];

        uint32_t h =
            (
                c * c * c +
                (
                    (uint32_t)i *
                    (uint32_t)
                    PRIMES[i % 29]
                )
            ) % 257;

        output[i] =
            (uint8_t)(h & 0xFF);
    }
}

static void temporal_fortress(
    uint8_t state[32]
) {

    uint8_t mem[16384];

    for(int i = 0;
        i < 16384;
        i++) {

        mem[i] =
            (uint8_t)(
                state[i % 32] ^
                (
                    (i * 31) & 0xFF
                )
            );
    }

    for(uint32_t r = 0;
        r < 524288;
        r++) {

        uint32_t idx =
            (
                (
                    r * 1315423911U
                ) ^
                (
                    (
                        (uint32_t)
                        state[
                            r % 32
                        ]
                    ) << 8
                ) ^
                (
                    (
                        (uint32_t)
                        state[
                            (
                                r + 7
                            ) % 32
                        ]
                    ) << 3
                )
            ) & 16383;

        mem[idx] ^=
            (uint8_t)(
                state[r % 32] ^
                state[
                    (
                        r + 11
                    ) % 32
                ]
            );

        uint32_t pos =
            r % 32;

        state[pos] =
    (uint8_t)(
        (uint8_t)(
            state[pos] +
            mem[idx]
        ) +
        state[
            (
                r + 5
            ) % 32
        ]
    );

        state[pos] =
            rotl8(
                state[pos],
                3
            );

        state[pos] ^=
            mem[
                (
                    idx ^ r
                ) & 16383
            ];
    }
}

void lox617_expand_key(
    const uint8_t *master,
    uint8_t *subkeys,
    uint32_t rounds
) {

    uint8_t sbox[256];

    lox617_generate_sbox(
        sbox
    );

    uint8_t stage[32];

    cubic_hardening(
        master,
        stage
    );

    temporal_fortress(
        stage
    );

    for(uint32_t r = 0;
        r < rounds;
        r++) {

        for(int b = 0;
            b < 4;
            b++) {

            uint8_t mix[8];

            for(int i = 0;
                i < 8;
                i++) {

                uint8_t a =
                    stage[
                        (
                            i +
                            r +
                            b * 3
                        ) % 32
                    ];

                uint8_t c =
                    stage[
                        (
                            i * 7 +
                            r * 13 +
                            b
                        ) % 32
                    ];

                mix[i] =
                    sbox[
                        (uint8_t)(
                            a ^
                            c ^
                            (uint8_t)r ^
                            (uint8_t)i ^
                            PRIMES[
                                (
                                    r + i
                                ) % 29
                            ]
                        )
                    ];
            }

            lox617_mds_apply(
                mix
            );

            for(int i = 0;
                i < 8;
                i++) {

                int idx =
                    (
                        b * 8 + i
                    ) % 32;

                stage[idx] ^=
                    mix[i];

                stage[idx] =
                    rotl8(
                        stage[idx],
                        (
                            r + i
                        ) % 7
                    );
            }
        }

        for(int i = 0;
            i < 32;
            i++) {

            uint8_t x =
                stage[
                    (
                        i + r
                    ) % 32
                ];

            uint8_t y =
                stage[
                    (
                        i * 5 + 11
                    ) % 32
                ];

            uint8_t z =
                sbox[
                    (uint8_t)(
                        x ^
                        y ^
                        stage[
                            (
                                i + 17
                            ) % 32
                        ]
                    )
                ];

            stage[i] =
                (uint8_t)(
                    z ^
                    rotl8(x, 1) ^
                    rotl8(y, 3)
                );

            subkeys[
                r * 32 + i
            ] = stage[i];
        }

        printf(
    "ROUND %u\n",
    r
);

for(int i = 0;
    i < 32;
    i++) {

    printf(
        "%02X",
        subkeys[
            r * 32 + i
        ]
    );
}

printf("\n\n");
    }
}