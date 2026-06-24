<?php

function lox617_gf_mul(
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

function lox617_mds_apply(
    array &$block
): void {

    $mds = [
        0x17,0x31,0x59,0x83,
        0xA7,0xC1,0xD3,0xEF
    ];

    $temp =
        array_fill(
            0,
            8,
            0
        );

    for($i = 0; $i < 8; $i++) {

        for($j = 0; $j < 8; $j++) {

            $temp[$i] ^=
                lox617_gf_mul(
                    $block[$j],
                    $mds[
                        ($i + $j) % 8
                    ]
                );
        }
    }

    for($i = 0; $i < 8; $i++) {

        $block[$i] =
            $temp[$i] & 0xFF;
    }
}