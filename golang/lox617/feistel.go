package lox617

import (
	"encoding/binary"
	"math/bits"
)

var PRIME_TABLE = [29]uint32{
	2, 3, 5, 7, 11, 13, 17, 19,
	23, 29, 31, 37, 41, 43,
	47, 53, 59, 61, 67, 71,
	73, 79, 83, 97, 103,
	107, 109, 113, 127,
}

func Lox617Encrypt(
	block *[32]byte,
	subkeys []byte,
	rounds int,
) {
	sbox := generateSBox()

	state := uint64(
		0x617617617617617,
	)

	for r := 0; r < rounds; r++ {

		for i := 0; i < 32; i++ {
			block[i] =
				sbox[block[i]]
		}

		for c := 0; c < 4; c++ {

			start := c * 8

			val :=
				binary.LittleEndian.Uint64(
					block[start : start+8],
				)

			shift :=
				PRIME_TABLE[r%29] % 64

			val =
				bits.RotateLeft64(
					val,
					int(shift),
				)

			binary.LittleEndian.PutUint64(
				block[start:start+8],
				val,
			)
		}

		for c := 0; c < 4; c++ {

			start := c * 8

			var local [8]byte

			copy(
				local[:],
				block[start:start+8],
			)

			lox617MDSApply(
				&local,
			)

			copy(
				block[start:start+8],
				local[:],
			)
		}

		for i := 0; i < 32; i++ {
			block[i] ^=
				subkeys[r*32+i]
		}

		entropyFold(
			&state,
		)

		for i := 0; i < 32; i++ {

			block[i] ^= byte(
				(state >> (i % 8)) & 0xFF,
			)
		}

		for c := 0; c < 4; c++ {

			start := c * 8

			var local [8]byte

			copy(
				local[:],
				block[start:start+8],
			)

			lox617CatMap(
				&local,
			)

			copy(
				block[start:start+8],
				local[:],
			)
		}
	}
}