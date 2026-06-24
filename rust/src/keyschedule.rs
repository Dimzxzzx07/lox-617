use crate::mds::lox617_mds_apply;
use crate::sbox::generate_sbox;

const PRIMES: [u8; 29] = [
    2,3,5,7,11,13,17,19,
    23,29,31,37,41,43,47,53,
    59,61,67,71,73,79,83,
    97,103,107,109,113,127
];

fn cubic_hardening(
    input: &[u8; 32]
) -> [u8; 32] {

    let mut out = [0u8; 32];

    for i in 0..32 {

        let c = input[i] as u32;

        let h =
            (
                c * c * c +
                (i as u32) *
                PRIMES[i % 29] as u32
            ) % 257;

        out[i] = h as u8;
    }

    out
}

fn temporal_fortress(
    state: &mut [u8; 32]
) {
    let mut mem =
        [0u8; 16384];

    for i in 0..16384 {

        mem[i] =
            state[i % 32] ^
            ((i * 31) & 0xFF) as u8;
    }

    for r in 0..524288u32 {

        let idx =
            (
                (r.wrapping_mul(
                    1315423911
                )) ^
                (
                    (state[
                        (r % 32) as usize
                    ] as u32) << 8
                ) ^
                (
                    (state[
                        ((r + 7) % 32)
                        as usize
                    ] as u32) << 3
                )
            ) & 16383;

        let idxu =
            idx as usize;

        mem[idxu] ^=
            state[
                (r % 32) as usize
            ] ^
            state[
                ((r + 11) % 32)
                as usize
            ];

        let pos =
            (r % 32) as usize;

        state[pos] =
            state[pos]
            .wrapping_add(
                mem[idxu]
            )
            .wrapping_add(
                state[
                    ((r + 5) % 32)
                    as usize
                ]
            );

        state[pos] =
            state[pos].rotate_left(3);

        state[pos] ^=
            mem[
                ((idx ^ r) & 16383)
                as usize
            ];
    }
}

pub fn lox617_expand_key(
    master: &[u8; 32],
    rounds: usize
) -> Vec<u8> {

    let sbox =
        generate_sbox();

    let mut stage =
        cubic_hardening(master);

    temporal_fortress(
        &mut stage
    );

    let mut subkeys =
        vec![0u8; rounds * 32];

    for r in 0..rounds {

        for b in 0..4 {

            let mut mix =
                [0u8; 8];

            for i in 0..8 {

                let a =
                    stage[
                        (i + r + b * 3)
                        % 32
                    ];

                let c =
                    stage[
                        (
                            i * 7 +
                            r * 13 +
                            b
                        ) % 32
                    ];

                mix[i] =
                    sbox[
                        (
                            a ^
                            c ^
                            r as u8 ^
                            i as u8 ^
                            PRIMES[
                                (r + i) % 29
                            ]
                        ) as usize
                    ];
            }

            lox617_mds_apply(
                &mut mix
            );

            for i in 0..8 {

                let idx =
                    (b * 8 + i)
                    % 32;

                stage[idx] ^=
                    mix[i];

                stage[idx] =
                    stage[idx]
                    .rotate_left(
                        (
                            (r + i) % 7
                        ) as u32
                    );
            }
        }

        for i in 0..32 {

            let x =
                stage[
                    (i + r) % 32
                ];

            let y =
                stage[
                    (i * 5 + 11)
                    % 32
                ];

            let z =
                sbox[
                    (
                        x ^
                        y ^
                        stage[
                            (i + 17)
                            % 32
                        ]
                    ) as usize
                ];

            stage[i] =
                z ^
                x.rotate_left(1) ^
                y.rotate_left(3);

            subkeys[
                r * 32 + i
            ] = stage[i];
        }

        println!("ROUND {}", r);

        for i in 0..32 {
            print!(
                "{:02X}",
                subkeys[r * 32 + i]
            );
        }

        println!("\n");
    }

    subkeys
}