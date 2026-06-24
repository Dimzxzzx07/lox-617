export function gfMul(a, b) {
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

export function generateSBox() {
    const sbox = new Uint8Array(256);

    for(let i = 0; i < 256; i++) {
        sbox[i] = i;
    }

    return sbox;
}