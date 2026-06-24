use crate::mds::lox617_mds_apply;
use crate::permutation::lox617_catmap;
use crate::sbox::generate_sbox;

const PRIMES: [u32; 29] = [
    2,3,5,7,11,13,17,19,
    23,29,31,37,41,43,47,53,
    59,61,67,71,73,79,83,
    97,103,107,109,113,127
];

fn round_function(
    half: &mut [u8; 16],
    subkey: &[u8],
    round: usize
) {

    let sbox =
        generate_sbox();

    for i in 0..16 {

        half[i] =
            sbox[
                (
                    half[i] ^
                    subkey[i]
                ) as usize
            ];
    }

    for c in 0..2 {

        let mut chunk =
            [0u8; 8];

        chunk.copy_from_slice(
            &half[
                c * 8..
                c * 8 + 8
            ]
        );

        lox617_mds_apply(
            &mut chunk
        );

        lox617_catmap(
            &mut chunk
        );

        let mut val =
            u64::from_le_bytes(
                chunk
            );

        val =
            val.rotate_left(
                PRIMES[
                    round % 29
                ]
            );

        chunk =
            val.to_le_bytes();

        for i in 0..8 {

            chunk[i] ^=
                subkey[
                    (c * 8 + i)
                    % 16
                ];
        }

        half[
            c * 8..
            c * 8 + 8
        ].copy_from_slice(
            &chunk
        );
    }
}

pub fn lox617_encrypt(
    block: &mut [u8; 32],
    subkeys: &[u8],
    rounds: usize
) {

    for i in 0..32 {

        block[i] ^=
            subkeys[i];
    }

    let mut left =
        [0u8; 16];

    let mut right =
        [0u8; 16];

    left.copy_from_slice(
        &block[..16]
    );

    right.copy_from_slice(
        &block[16..]
    );

    for r in 0..rounds {

        let mut temp =
            right;

        round_function(
            &mut temp,
            &subkeys[
                r * 32..
                r * 32 + 32
            ],
            r
        );

        for i in 0..16 {

            temp[i] ^= left[i];
        }

        left = right;
        right = temp;
    }

    block[..16]
        .copy_from_slice(
            &left
        );

    block[16..]
        .copy_from_slice(
            &right
        );

    for i in 0..32 {

        block[i] ^=
            subkeys[
                (rounds - 1)
                * 32 + i
            ];
    }
}

pub fn lox617_decrypt(
    block: &mut [u8; 32],
    subkeys: &[u8],
    rounds: usize
) {

    for i in 0..32 {

        block[i] ^=
            subkeys[
                (rounds - 1)
                * 32 + i
            ];
    }

    let mut left =
        [0u8; 16];

    let mut right =
        [0u8; 16];

    left.copy_from_slice(
        &block[..16]
    );

    right.copy_from_slice(
        &block[16..]
    );

    for r in (0..rounds).rev() {

        let mut temp =
            left;

        round_function(
            &mut temp,
            &subkeys[
                r * 32..
                r * 32 + 32
            ],
            r
        );

        for i in 0..16 {

            temp[i] ^= right[i];
        }

        right = left;
        left = temp;
    }

    block[..16]
        .copy_from_slice(
            &left
        );

    block[16..]
        .copy_from_slice(
            &right
        );

    for i in 0..32 {

        block[i] ^=
            subkeys[i];
    }
}