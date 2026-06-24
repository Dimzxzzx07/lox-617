#ifndef LOX617_H
#define LOX617_H

#include <stdint.h>
#include <stddef.h>

void lox617_encrypt(
    uint8_t *block,
    const uint8_t *key,
    size_t rounds
);

void lox617_decrypt(
    uint8_t *block,
    const uint8_t *key,
    size_t rounds
);

#endif