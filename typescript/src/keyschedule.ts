const primes = [
    2,3,5,7,11,13,17,19,
    23,29,31,37,41,43,47,53,
    59,61,67,71,73,79,83,
    97,103,107,109,113,127
];

function temporalFortress(
    state: Uint8Array
): void {

    const mem =
        new Uint8Array(16384);

    for(let i = 0; i < 16384; i++) {
        mem[i] =
            state[i % 32] ^ i;
    }

    for(let r = 0; r < 262144; r++) {
        const idx =
            (r * 1315423911) & 16383;

        mem[idx] ^=
            state[r % 32];

        state[r % 32] ^=
            mem[idx];
    }
}

export function expandKey(
    key: Uint8Array,
    rounds: number
): Uint8Array {

    const stage =
        new Uint8Array(32);

    for(let i = 0; i < 32; i++) {
        const c = key[i];

        const h =
            (c * c * c +
            i * primes[i % 29]) % 257;

        stage[i] = h & 0xFF;
    }

    temporalFortress(stage);

    const subkeys =
        new Uint8Array(rounds * 32);

    for(let r = 0; r < rounds; r++) {
        for(let i = 0; i < 32; i++) {
            subkeys[r * 32 + i] =
                stage[i] ^
                ((r + i) * 17);
        }
    }

    return subkeys;
}