<?php

require_once "SBox.php";
require_once "MDS.php";
require_once "NLFSR.php";
require_once "Permutation.php";
require_once "KeySchedule.php";
require_once "Hash.php";

class LOX617 {

    private static array $primes = [
        2,3,5,7,11,13,17,19,
        23,29,31,37,41,43,
        47,53,59,61,67,71,
        73,79,83,97,103,
        107,109,113,127
    ];

    public static function encrypt(
        string $data,
        string $key,
        int $rounds = 16
    ): string {

        $sbox =
            SBox::generate();

        $subkeys =
            KeySchedule::derive(
                $key,
                $rounds
            );

        $block =
            array_values(
                unpack(
                    "C*",
                    str_pad(
                        substr(
                            $data,
                            0,
                            32
                        ),
                        32,
                        "\0"
                    )
                )
            );

        $state =
            0x617617617617617;

        for($r = 0; $r < $rounds; $r++) {

            for($i = 0; $i < 32; $i++) {
                $block[$i] =
                    $sbox[
                        $block[$i]
                    ];
            }

            $shift =
                self::$primes[
                    $r % 29
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

            for($c = 0; $c < 4; $c++) {

                $slice =
                    array_slice(
                        $block,
                        $c * 8,
                        8
                    );

                $slice =
                    MDS::mix($slice);

                for($j = 0; $j < 8; $j++) {
                    $block[
                        $c * 8 + $j
                    ] = $slice[$j];
                }
            }

            for($i = 0; $i < 32; $i++) {
                $block[$i] ^=
                    $subkeys[$r][$i];
            }

            NLFSR::fold($state);

            for($i = 0; $i < 32; $i++) {
                $block[$i] ^=
                    (($state >> ($i % 8))
                    & 0xFF);
            }

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
                    ] = $slice[$j];
                }
            }
        }

        return strtoupper(
            bin2hex(
                implode(
                    array_map(
                        "chr",
                        $block
                    )
                )
            )
        );
    }
}