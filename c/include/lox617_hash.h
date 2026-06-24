#ifndef LOX617_HASH_H
#define LOX617_HASH_H

#include <stdint.h>

void lox617_blake3_seal(
    const uint8_t *data,
    uint32_t len,
    uint8_t *out
);

#endif