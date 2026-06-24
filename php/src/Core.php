<?php

require_once "SBox.php";
require_once "MDS.php";
require_once "NLFSR.php";
require_once "Permutation.php";

class Core {

    private static array $primes = [
        2,3,5,7,11,13,17,19,
        23,29,31,37,41,43,
        47,53,59,61,67,71,
        73,79,83,97,103,
        107,109,113,127
    ];

    public static function substitute(
        array $block,
        array $sbox
    ): array {

        for($i = 0; $i < 32; $i++) {
            $block[$i] =
                $sbox[
                    $block[$i]
                ];
        }

        return $block;
    }

    public static function rotate(
        array $block,
        int $round
    ): array {

        $shift =
            self::$primes[
                $round % 29
            ] % 64;

        for($c = 0; $c < 4; $c++) {

            $val = 0;

            for($j = 0; $j < 8; $j++) {

                $val |=
                    $block[
                        $c * 8 + $j
                    ] << ($j * 8);
            }

            $val =
                (($val << $shift) |
                ($val >> (64 - $shift)));

            for($j = 0; $j < 8; $j++) {

                $block[
                    $c * 8 + $j
                ] =
                    ($val >> ($j * 8))
                    & 0xFF;
            }
        }

        return $block;
    }

    public static function mix(
        array $block
    ): array {

        for($c = 0; $c < 4; $c++) {

            $slice =
                array_slice(
                    $block,
                    $c * 8,
                    8
                );

            $slice =
                MDS::mix(
                    $slice
                );

            for($j = 0; $j < 8; $j++) {

                $block[
                    $c * 8 + $j
                ] =
                    $slice[$j];
            }
        }

        return $block;
    }

    public static function xorSubkey(
        array $block,
        array $subkey
    ): array {

        for($i = 0; $i < 32; $i++) {
            $block[$i] ^=
                $subkey[$i];
        }

        return $block;
    }

    public static function entropy(
        array $block,
        int &$state
    ): array {

        NLFSR::fold(
            $state
        );

        for($i = 0; $i < 32; $i++) {

            $block[$i] ^=
                (($state >> ($i % 8))
                & 0xFF);
        }

        return $block;
    }

    public static function permute(
        array $block
    ): array {

        for($c = 0; $c < 4; $c++) {

            $slice =
                array_slice(
                    $block,
                    $c * 8,
                    8
                );

            $slice =
                Permutation::torus(
                    $slice
                );

            for($j = 0; $j < 8; $j++) {

                $block[
                    $c * 8 + $j
                ] =
                    $slice[$j];
            }
        }

        return $block;
    }
}