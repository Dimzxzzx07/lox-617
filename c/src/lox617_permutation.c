#include "../include/lox617_permutation.h"

void lox617_catmap(uint8_t *block) {
    uint8_t temp[64] = {0};

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {

            int bit =
                (block[i] >> j) & 1;

            int new_x =
                (2 * i + j) % 8;

            int new_y =
                (i + j) % 8;

            temp[new_x] |=
                (bit << new_y);
        }
    }

    for(int i = 0; i < 8; i++) {
        block[i] = temp[i];
    }
}