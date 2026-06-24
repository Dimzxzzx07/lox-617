<?php

function lox617_cubic_hardening(
    array $input
): array {

    $primes = [
        2,3,5,7,11,13,17,19,
        23,29,31,37,41,43,47,53,
        59,61,67,71,73,79,83,
        97,103,107,109,113,127
    ];

    $out = [];

    for($i = 0; $i < 32; $i++) {

        $c =
            $input[$i];

        $h =
            (($c * $c * $c) +
            ($i * $primes[$i % 29]))
            % 257;

        $out[$i] =
            $h & 0xFF;
    }

    return $out;
}

function lox617_temporal_fortress(
    array &$state
): void {

    $mem =
        array_fill(
            0,
            16384,
            0
        );

    for($i = 0; $i < 16384; $i++) {

        $mem[$i] =
            $state[$i % 32] ^ $i;
    }

    for($r = 0; $r < 262144; $r++) {

        $idx =
            ($r * 1315423911)
            & 16383;

        $mem[$idx] ^=
            $state[$r % 32];

        $state[$r % 32] ^=
            $mem[$idx];
    }
}

function lox617_expand_key(
    array $master,
    int $rounds
): array {

    $stage =
        lox617_cubic_hardening(
            $master
        );

    lox617_temporal_fortress(
        $stage
    );

    $subkeys = [];

    for($r = 0; $r < $rounds; $r++) {

        for($i = 0; $i < 32; $i++) {

            $subkeys[] =
                $stage[$i] ^
                (($r + $i) * 17);
        }
    }

    return $subkeys;
}