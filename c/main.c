#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

void lox617_encrypt_feistel(
    uint8_t block[32],
    uint8_t *subkeys,
    int rounds
);

void lox617_decrypt_feistel(
    uint8_t block[32],
    uint8_t *subkeys,
    int rounds
);

void lox617_expand_key(
    const uint8_t *master,
    uint8_t *subkeys,
    uint32_t rounds
);

static void key_to_32(
    const char *key,
    uint8_t out[32]
) {

    int len =
        strlen(key);

    for(int i = 0; i < 32; i++) {

        out[i] =
            key[
                i % len
            ];
    }
}

static void text_to_block(
    const char *text,
    uint8_t block[32]
) {

    memset(
        block,
        0,
        32
    );

    int len =
        strlen(text);

    if(len > 32) {
        len = 32;
    }

    memcpy(
        block,
        text,
        len
    );
}

static void hex_to_block(
    const char *hex,
    uint8_t block[32]
) {

    for(int i = 0; i < 32; i++) {

        sscanf(
            &hex[i * 2],
            "%2hhx",
            &block[i]
        );
    }
}

static void print_hex(
    uint8_t block[32]
) {

    for(int i = 0; i < 32; i++) {

        printf(
            "%02X",
            block[i]
        );
    }

    printf("\n");
}

int main(
    int argc,
    char **argv
) {

    if(argc < 6) {

        printf(
            "Usage:\n"
        );

        printf(
            "enc -key <key> -text <text>\n"
        );

        printf(
            "dec -key <key> -hex <hex>\n"
        );

        return 0;
    }

    char *mode =
        argv[1];

    char *key_input =
        argv[3];

    if(strlen(key_input) < 15) {

        printf(
            "Key minimum 15 chars\n"
        );

        return 0;
    }

    uint8_t key[32];

    key_to_32(
        key_input,
        key
    );

    uint8_t subkeys[
        16 * 32
    ];

    lox617_expand_key(
        key,
        subkeys,
        16
    );

    uint8_t block[32];

    if(strcmp(mode, "enc") == 0) {

        text_to_block(
            argv[5],
            block
        );

        lox617_encrypt_feistel(
            block,
            subkeys,
            16
        );

        print_hex(
            block
        );

    } else if(
        strcmp(mode, "dec") == 0
    ) {

        hex_to_block(
            argv[5],
            block
        );

        lox617_decrypt_feistel(
            block,
            subkeys,
            16
        );

        printf(
            "%s\n",
            block
        );
    }

    return 0;
}