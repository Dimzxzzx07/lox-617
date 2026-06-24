pub fn gf_mul(mut a: u8, mut b: u8) -> u8 {
    let mut p: u8 = 0;

    for _ in 0..8 {

        if b & 1 != 0 {
            p ^= a;
        }

        let hi = a & 0x80;

        a <<= 1;

        if hi != 0 {
            a ^= 0x1B;
        }

        b >>= 1;
    }

    p
}

pub fn gf_inv(a: u8) -> u8 {

    if a == 0 {
        return 0;
    }

    let mut t: u8 = 1;

    for _ in 0..254 {
        t = gf_mul(t, a);
    }

    t
}

pub fn generate_sbox() -> [u8; 256] {

    let mut sbox = [0u8; 256];

    for i in 0..256 {

        let x =
            gf_inv(i as u8);

        let y =
            x ^
            x.rotate_left(1) ^
            x.rotate_left(2) ^
            0x63;

        sbox[i] = y;
    }

    sbox
}