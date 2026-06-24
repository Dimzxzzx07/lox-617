export function catMap(block) {
    const temp = new Uint8Array(8);

    for(let i = 0; i < 8; i++) {
        for(let j = 0; j < 8; j++) {
            const bit =
                (block[i] >> j) & 1;

            const newX =
                (2 * i + j) % 8;

            const newY =
                (i + j) % 8;

            temp[newX] |=
                bit << newY;
        }
    }

    block.set(temp);
}