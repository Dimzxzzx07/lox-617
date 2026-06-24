import { generateSBox } from "./sbox.js";

export function encryptText(key, text) {
    const sbox = generateSBox();

    const out = [];

    for(let i = 0; i < text.length; i++) {
        const v =
            text.charCodeAt(i) ^
            key.charCodeAt(i % key.length);

        out.push(
            sbox[v & 0xFF]
                .toString(16)
                .padStart(2, "0")
        );
    }

    return out.join("").toUpperCase();
}