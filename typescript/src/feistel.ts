import { generateSBox } from "./sbox";
import { mdsApply } from "./mds";
import { catMap } from "./permutation";

function roundFunction(
    half: Uint8Array,
    subkey: Uint8Array
): Uint8Array {

    const sbox = generateSBox();

    const out =
        new Uint8Array(half);

    for(let i = 0; i < 16; i++) {
        out[i] =
            sbox[
                (out[i] ^ subkey[i]) & 0xFF
            ];
    }

    for(let c = 0; c < 2; c++) {
        const chunk =
            out.slice(c * 8, c * 8 + 8);

        mdsApply(chunk);
        catMap(chunk);

        out.set(chunk, c * 8);
    }

    return out;
}

export function encryptFeistel(
    block: Uint8Array,
    subkeys: Uint8Array,
    rounds: number
): Uint8Array {

    let left = block.slice(0, 16);
    let right = block.slice(16, 32);

    for(let r = 0; r < rounds; r++) {
        const f =
            roundFunction(
                right,
                subkeys.slice(r * 32, r * 32 + 16)
            );

        const newRight =
            new Uint8Array(16);

        for(let i = 0; i < 16; i++) {
            newRight[i] =
                left[i] ^ f[i];
        }

        left = right;
        right = newRight;
    }

    const out =
        new Uint8Array(32);

    out.set(left, 0);
    out.set(right, 16);

    return out;
}