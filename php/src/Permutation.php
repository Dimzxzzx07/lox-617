<?php

function lox617_catmap(
    array &$block
): void {

    $temp =
        array_fill(
            0,
            8,
            0
        );

    for($i = 0; $i < 8; $i++) {

        for($j = 0; $j < 8; $j++) {

            $bit =
                ($block[$i] >> $j) & 1;

            $newX =
                (2 * $i + $j) % 8;

            $newY =
                ($i + $j) % 8;

            $temp[$newX] |=
                ($bit << $newY);
        }
    }

    for($i = 0; $i < 8; $i++) {

        $block[$i] =
            $temp[$i] & 0xFF;
    }
}