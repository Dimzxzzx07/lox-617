import { expandKey } from "./keyschedule";
import { encryptFeistel } from "./feistel";

export function encryptText(
    key: string,
    text: string
): string {

    const keyBytes =
        new Uint8Array(32);

    for(let i = 0; i < 32; i++) {
        keyBytes[i] =
            key.charCodeAt(i % key.length);
    }

    const block =
        new Uint8Array(32);

    for(let i = 0; i < text.length && i < 32; i++) {
        block[i] =
            text.charCodeAt(i);
    }

    const subkeys =
        expandKey(keyBytes, 16);

    const enc =
        encryptFeistel(block, subkeys, 16);

    return Array.from(enc)
        .map(v => v.toString(16).padStart(2, "0"))
        .join("")
        .toUpperCase();
}