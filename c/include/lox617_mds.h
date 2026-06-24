#ifndef LOX617_MDS_H
#define LOX617_MDS_H

#include <stdint.h>

uint8_t lox_gf256_mul(uint8_t a, uint8_t b);
void lox617_mds_apply(uint8_t *block);

#endif