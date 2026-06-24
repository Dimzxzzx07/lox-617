<?php

function lox617_gf256_mul(
    int $a,
    int $b
): int {

    $p = 0;

    for($i = 0; $i < 8; $i++) {

        if($b & 1) {
            $p ^= $a;
        }

        $hi =
            $a & 0x80;

        $a =
            ($a << 1) & 0xFF;

        if($hi) {
            $a ^= 0x1B;
        }

        $b >>= 1;
    }

    return $p;
}

function lox617_gf256_inv(
    int $x
): int {

    if($x === 0) {
        return 0;
    }

    for($i = 1; $i < 256; $i++) {

        if(
            lox617_gf256_mul(
                $x,
                $i
            ) === 1
        ) {

            return $i;
        }
    }

    return 0;
}

function lox617_generate_sbox(): array {

    $sbox = [];

    for($i = 0; $i < 256; $i++) {

        $inv =
            lox617_gf256_inv(
                $i
            );

        $x =
            $inv;

        $y =
            $x ^
            (($x << 1) & 0xFF) ^
            (($x << 2) & 0xFF) ^
            (($x << 3) & 0xFF) ^
            (($x << 4) & 0xFF) ^
            0x63;

        $sbox[$i] =
            $y & 0xFF;
    }

    return $sbox;
}