import { generateSBox } from "./sbox.js";
import { mdsApply } from "./mds.js";
import { catMap } from "./permutation.js";

function roundFunction(
    half,
    subkey
) {
    const sbox =
        generateSBox();

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
    block,
    subkeys,
    rounds
) {
    let left =
        block.slice(0, 16);

    let right =
        block.slice(16, 32);

    for(let r = 0; r < rounds; r++) {
        const f =
            roundFunction(
                right,
                subkeys.slice(
                    r * 32,
                    r * 32 + 16
                )
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