fn rotl32(
    v: u32,
    n: u32
) -> u32 {

    (v << n) |
    (v >> (32 - n))
}

pub fn blake3_seal(
    data: &[u8]
) -> [u8; 32] {

    let mut state: [u32; 8] = [
        0x6A09E667,
        0xBB67AE85,
        0x3C6EF372,
        0xA54FF53A,
        0x510E527F,
        0x9B05688C,
        0x1F83D9AB,
        0x5BE0CD19
    ];

    for (i, b) in
        data.iter().enumerate()
    {

        let idx =
            i % 8;

        state[idx] ^=
            *b as u32;

        state[idx] =
            rotl32(
                state[idx],
                ((i % 31) + 1)
                as u32
            );

        state[idx] =
            state[idx]
            .wrapping_add(
                0x9E3779B1
            );

        state[idx] ^=
            state[
                (idx + 3) % 8
            ];
    }

    let mut out =
        [0u8; 32];

    for i in 0..8 {

        out[i * 4] =
            ((state[i] >> 24)
            & 0xFF) as u8;

        out[i * 4 + 1] =
            ((state[i] >> 16)
            & 0xFF) as u8;

        out[i * 4 + 2] =
            ((state[i] >> 8)
            & 0xFF) as u8;

        out[i * 4 + 3] =
            (state[i]
            & 0xFF) as u8;
    }

    out
}