#ifndef LOX617_CORE_H
#define LOX617_CORE_H

#include <stdint.h>

void lox617_substitute(uint8_t *block);
void lox617_prime_rotate(uint64_t *state, uint32_t round);
void lox617_mds_mix(uint8_t *block);
void lox617_xor_subkey(uint8_t *block, const uint8_t *subkey);
void lox617_entropy_fold(uint64_t *state);
void lox617_torus_permute(uint8_t *block);

#endif