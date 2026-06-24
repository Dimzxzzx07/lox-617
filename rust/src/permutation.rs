pub fn lox617_catmap(
    block: &mut [u8; 8]
) {

    let mut temp =
        [0u8; 8];

    for i in 0..8 {

        for j in 0..8 {

            let bit =
                (block[i] >> j)
                & 1;

            let new_x =
                (2 * i + j) % 8;

            let new_y =
                (i + j) % 8;

            temp[new_x] |=
                bit << new_y;
        }
    }

    *block = temp;
}