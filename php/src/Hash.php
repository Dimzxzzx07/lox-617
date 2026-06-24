<?php

class HashLOX {

    public static function seal(
        string $data
    ): string {

        return hash(
            "sha3-256",
            $data,
            true
        );
    }
}