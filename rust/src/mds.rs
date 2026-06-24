pub fn lox_gf256_mul(
    mut a: u8,
    mut b: u8
) -> u8 {

    let mut p = 0u8;

    for _ in 0..8 {

        if (b & 1) != 0 {
            p ^= a;
        }

        let hi =
            a & 0x80;

        a <<= 1;

        if hi != 0 {
            a ^= 0x1B;
        }

        b >>= 1;
    }

    p
}

pub fn lox617_mds_apply(
    block: &mut [u8; 8]
) {

    let mds: [u8; 8] = [
        0x17, 0x31,
        0x59, 0x83,
        0xA7, 0xC1,
        0xD3, 0xEF
    ];

    let mut temp =
        [0u8; 8];

    for i in 0..8 {

        for j in 0..8 {

            temp[i] ^=
                lox_gf256_mul(
                    block[j],
                    mds[
                        (i + j) % 8
                    ]
                );
        }
    }

    *block = temp;
}