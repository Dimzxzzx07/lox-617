package lox617

import "fmt"

var primesKS = [29]byte{
	2, 3, 5, 7, 11, 13, 17, 19,
	23, 29, 31, 37, 41, 43, 47, 53,
	59, 61, 67, 71, 73, 79, 83,
	97, 103, 107, 109, 113, 127,
}

func rotl8(x byte, s byte) byte {
	s &= 7
	return (x << s) | (x >> (8 - s))
}

func cubicHardening(input *[32]byte) [32]byte {

	var out [32]byte

	for i := 0; i < 32; i++ {

		c := uint32(input[i])

		h :=
			(c*c*c +
				uint32(i)*uint32(primesKS[i%29])) % 257

		out[i] = byte(h)
	}

	return out
}

func temporalFortress(state *[32]byte) {

	mem := make([]byte, 16384)

	for i := 0; i < 16384; i++ {

		mem[i] =
			state[i%32] ^
				byte((i*31)&0xFF)
	}

	for r := uint32(0); r < 524288; r++ {

		idx :=
			((r * 1315423911) ^
				(uint32(state[r%32]) << 8) ^
				(uint32(state[(r+7)%32]) << 3)) & 16383

		mem[idx] ^=
			state[r%32] ^
				state[(r+11)%32]

		pos := r % 32

		state[pos] =
			state[pos] +
				mem[idx] +
				state[(r+5)%32]

		state[pos] =
			rotl8(state[pos], 3)

		state[pos] ^=
			mem[(idx^r)&16383]
	}
}

func Lox617ExpandKey(
	master *[32]byte,
	rounds int,
) []byte {

	sbox := generateSBox()

	stage :=
		cubicHardening(master)

	temporalFortress(&stage)

	subkeys :=
		make([]byte, rounds*32)

	for r := 0; r < rounds; r++ {

		for b := 0; b < 4; b++ {

			var mix [8]byte

			for i := 0; i < 8; i++ {

				a :=
					stage[(i+r+b*3)%32]

				c :=
					stage[(i*7+r*13+b)%32]

				mix[i] =
					sbox[
						a^
							c^
							byte(r)^
							byte(i)^
							primesKS[(r+i)%29],
					]
			}

			lox617MDSApply(&mix)

			for i := 0; i < 8; i++ {

				idx :=
					(b*8 + i) % 32

				stage[idx] ^= mix[i]

				stage[idx] =
					rotl8(
						stage[idx],
						byte((r+i)%7),
					)
			}
		}

		for i := 0; i < 32; i++ {

			x :=
				stage[(i+r)%32]

			y :=
				stage[(i*5+11)%32]

			z :=
				sbox[
					x ^
						y ^
						stage[(i+17)%32],
				]

			stage[i] =
				z ^
					rotl8(x, 1) ^
					rotl8(y, 3)

			subkeys[r*32+i] =
				stage[i]
		}

		fmt.Printf("ROUND %d\n", r)

		for i := 0; i < 32; i++ {

			fmt.Printf(
				"%02X",
				subkeys[r*32+i],
			)
		}

		fmt.Printf("\n\n")
	}

	return subkeys
}