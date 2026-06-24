import { gfMul } from "./sbox";

const mds = [
    0x17,0x31,0x59,0x83,
    0xA7,0xC1,0xD3,0xEF
];

export function mdsApply(block: Uint8Array): void {
    const temp = new Uint8Array(8);

    for(let i = 0; i < 8; i++) {
        for(let j = 0; j < 8; j++) {
            temp[i] ^=
                gfMul(
                    block[j],
                    mds[(i + j) % 8]
                );
        }
    }

    block.set(temp);
}