<?php

require_once __DIR__ . "/src/KeySchedule.php";
require_once __DIR__ . "/src/Feistel.php";

if($argc < 6) {

    echo "Usage:\n";
    echo "enc -key <key> -text <text>\n";
    echo "dec -key <key> -hex <hex>\n";

    exit;
}

$mode =
    $argv[1];

$keyInput =
    $argv[3];

if(strlen($keyInput) < 15) {

    echo "Key minimum 15 chars\n";

    exit;
}

$key = [];

for($i = 0; $i < 32; $i++) {

    $key[$i] =
        ord(
            $keyInput[
                $i % strlen($keyInput)
            ]
        );
}

$subkeys =
    lox617_expand_key(
        $key,
        16
    );

if($mode === "enc") {

    $text =
        $argv[5];

    $block =
        array_fill(
            0,
            32,
            0
        );

    for($i = 0; $i < strlen($text) && $i < 32; $i++) {

        $block[$i] =
            ord($text[$i]);
    }

    lox617_encrypt_feistel(
        $block,
        $subkeys,
        16
    );

    foreach($block as $b) {
        printf("%02X", $b);
    }

    echo PHP_EOL;

} else if(
    $mode === "dec"
) {

    $hex =
        $argv[5];

    $block = [];

    for($i = 0; $i < 64; $i += 2) {

        $block[] =
            hexdec(
                substr(
                    $hex,
                    $i,
                    2
                )
            );
    }

    lox617_decrypt_feistel(
        $block,
        $subkeys,
        16
    );

    foreach($block as $b) {

        if($b !== 0) {
            echo chr($b);
        }
    }

    echo PHP_EOL;
}