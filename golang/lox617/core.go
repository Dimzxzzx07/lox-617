package lox617

import "encoding/binary"

var primesCore = [29]uint32{
	2, 3, 5, 7, 11, 13, 17, 19,
	23, 29, 31, 37, 41, 43,
	47, 53, 59, 61, 67, 71,
	73, 79, 83, 97, 103,
	107, 109, 113, 127,
}

func Encrypt(
	block *[32]byte,
	key *[32]byte,
	rounds int,
) {

	sbox :=
		generateSBox()

	subkeys :=
		Lox617ExpandKey(
			key,
			rounds,
		)

	var state uint64 =
		0x617617617617617

	for r := 0; r < rounds; r++ {

		for i := 0; i < 32; i++ {

			block[i] =
				sbox[
					block[i],
				]
		}

		for i := 0; i < 4; i++ {

			start := i * 8

			val :=
				binary.LittleEndian.Uint64(
					block[start : start+8],
				)

			shift :=
				primesCore[r%29] % 64

			val =
				(val << shift) |
					(val >> (64 - shift))

			binary.LittleEndian.PutUint64(
				block[start:start+8],
				val,
			)
		}

		for i := 0; i < 4; i++ {

			var local [8]byte

			copy(
				local[:],
				block[i*8:(i+1)*8],
			)

			lox617MDSApply(
				&local,
			)

			copy(
				block[i*8:(i+1)*8],
				local[:],
			)
		}

		for i := 0; i < 32; i++ {

			block[i] ^=
				subkeys[
					r*32+i,
				]
		}

		entropyFold(
			&state,
		)

		for i := 0; i < 32; i++ {

			block[i] ^=
				byte(
					state >> (i % 8),
				)
		}

		for i := 0; i < 4; i++ {

			var local [8]byte

			copy(
				local[:],
				block[i*8:(i+1)*8],
			)

			lox617CatMap(
				&local,
			)

			copy(
				block[i*8:(i+1)*8],
				local[:],
			)
		}
	}
}