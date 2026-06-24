<?php

require_once __DIR__ . "/SBox.php";
require_once __DIR__ . "/MDS.php";
require_once __DIR__ . "/Permutation.php";

function lox617_round_function(
    array $half,
    array $subkey
): array {

    $sbox =
        lox617_generate_sbox();

    $out =
        $half;

    for($i = 0; $i < 16; $i++) {

        $idx =
            (
                $out[$i] ^
                $subkey[$i]
            ) & 0xFF;

        $out[$i] =
            $sbox[$idx];
    }

    for($c = 0; $c < 2; $c++) {

        $chunk =
            array_slice(
                $out,
                $c * 8,
                8
            );

        lox617_mds_apply(
            $chunk
        );

        lox617_catmap(
            $chunk
        );

        for($i = 0; $i < 8; $i++) {

            $out[
                $c * 8 + $i
            ] = $chunk[$i];
        }
    }

    return $out;
}

function lox617_encrypt_feistel(
    array &$block,
    array $subkeys,
    int $rounds
): void {

    $left =
        array_slice(
            $block,
            0,
            16
        );

    $right =
        array_slice(
            $block,
            16,
            16
        );

    for($r = 0; $r < $rounds; $r++) {

        $sub =
            array_slice(
                $subkeys,
                $r * 32,
                16
            );

        $f =
            lox617_round_function(
                $right,
                $sub
            );

        $newRight = [];

        for($i = 0; $i < 16; $i++) {

            $newRight[$i] =
                $left[$i] ^ $f[$i];
        }

        $left = $right;
        $right = $newRight;
    }

    $block =
        array_merge(
            $left,
            $right
        );
}

function lox617_decrypt_feistel(
    array &$block,
    array $subkeys,
    int $rounds
): void {

    $left =
        array_slice(
            $block,
            0,
            16
        );

    $right =
        array_slice(
            $block,
            16,
            16
        );

    for($r = $rounds - 1; $r >= 0; $r--) {

        $sub =
            array_slice(
                $subkeys,
                $r * 32,
                16
            );

        $f =
            lox617_round_function(
                $left,
                $sub
            );

        $newLeft = [];

        for($i = 0; $i < 16; $i++) {

            $newLeft[$i] =
                $right[$i] ^ $f[$i];
        }

        $right = $left;
        $left = $newLeft;
    }

    $block =
        array_merge(
            $left,
            $right
        );
}