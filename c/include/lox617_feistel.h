#ifndef LOX617_FEISTEL_H
#define LOX617_FEISTEL_H

#include <stdint.h>

void lox617_encrypt_feistel(
    uint8_t *block,
    uint8_t *subkeys,
    uint32_t rounds
);

void lox617_decrypt_feistel(
    uint8_t *block,
    uint8_t *subkeys,
    uint32_t rounds
);

#endif