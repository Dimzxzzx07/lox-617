export function gfMul(a: number, b: number): number {
    let p = 0;

    for(let i = 0; i < 8; i++) {
        if(b & 1) {
            p ^= a;
        }

        const hi = a & 0x80;

        a = (a << 1) & 0xFF;

        if(hi) {
            a ^= 0x1B;
        }

        b >>= 1;
    }

    return p & 0xFF;
}

export function gfInv(x: number): number {
    if(x === 0) {
        return 0;
    }

    for(let i = 1; i < 256; i++) {
        if(gfMul(x, i) === 1) {
            return i;
        }
    }

    return 0;
}

export function generateSBox(): Uint8Array {
    const sbox = new Uint8Array(256);

    for(let i = 0; i < 256; i++) {
        const inv = gfInv(i);

        const y =
            inv ^
            ((inv << 1) & 0xFF) ^
            ((inv << 2) & 0xFF) ^
            ((inv << 3) & 0xFF) ^
            ((inv << 4) & 0xFF) ^
            0x63;

        sbox[i] = y & 0xFF;
    }

    return sbox;
}