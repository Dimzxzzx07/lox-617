use crate::sbox::generate_sbox;
use crate::mds::lox617_mds_apply;
use crate::nlfsr::entropy_fold;
use crate::permutation::lox617_catmap;
use crate::keyschedule::lox617_expand_key;

const PRIMES: [u32; 29] = [
    2,3,5,7,11,13,17,19,
    23,29,31,37,41,43,
    47,53,59,61,67,71,
    73,79,83,97,103,
    107,109,113,127
];

pub fn encrypt(
    block: &mut [u8; 32],
    key: &[u8; 32],
    rounds: usize
) {

    let sbox =
        generate_sbox();

    let subkeys =
        lox617_expand_key(
            key,
            rounds
        );

    let mut state: u64 =
        0x617617617617617;

    for r in 0..rounds {

        for i in 0..32 {
            block[i] =
                sbox[
                    block[i] as usize
                ];
        }

        for chunk in
            block.chunks_mut(8)
        {

            let mut val =
                u64::from_le_bytes(
                    chunk.try_into().unwrap()
                );

            let shift =
                PRIMES[r % 29] % 64;

            val =
                val.rotate_left(
                    shift
                );

            chunk.copy_from_slice(
    &val.to_le_bytes()
);
        }

        for chunk in
            block.chunks_mut(8)
        {

            let mut local =
                [0u8; 8];

            local.copy_from_slice(
                chunk
            );

            lox617_mds_apply(
                &mut local
            );

            chunk.copy_from_slice(
                &local
            );
        }

        for i in 0..32 {
            block[i] ^=
                subkeys[
                    r * 32 + i
                ];
        }

        entropy_fold(
            &mut state
        );

        for i in 0..32 {
            block[i] ^=
                (state >> (i % 8))
                as u8;
        }

        for chunk in
            block.chunks_mut(8)
        {

            let mut local =
                [0u8; 8];

            local.copy_from_slice(
                chunk
            );

            lox617_catmap(
                &mut local
            );

            chunk.copy_from_slice(
                &local
            );
        }
    }
}