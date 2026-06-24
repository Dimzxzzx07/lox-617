#ifndef LOX617_KEYSCHEDULE_H
#define LOX617_KEYSCHEDULE_H

#include <stdint.h>

void lox617_expand_key(
    const uint8_t *master,
    uint8_t *subkeys,
    uint32_t rounds
);

#endif