pub fn entropy_fold(
    state: &mut u64
) {

    let mut fb =
        (*state >> 63) & 1;

    fb ^= (*state >> 61) & 1;

    fb ^= (*state >> 59) & 1;

    fb ^= ((*state >> 55) & 1)
        & ((*state >> 47) & 1);

    fb ^= ((*state >> 31) & 1)
        | ((*state >> 23) & 1);

    *state =
        (*state << 1) | fb;
}