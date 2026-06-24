package lox617

func loxGF256Mul(a, b byte) byte {

	var p byte = 0

	for i := 0; i < 8; i++ {

		if (b & 1) != 0 {
			p ^= a
		}

		hi := a & 0x80

		a <<= 1

		if hi != 0 {
			a ^= 0x1B
		}

		b >>= 1
	}

	return p
}

func lox617MDSApply(
	block *[8]byte,
) {

	mds := [8]byte{
		0x17, 0x31,
		0x59, 0x83,
		0xA7, 0xC1,
		0xD3, 0xEF,
	}

	var temp [8]byte

	for i := 0; i < 8; i++ {

		for j := 0; j < 8; j++ {

			temp[i] ^=
				loxGF256Mul(
					block[j],
					mds[(i+j)%8],
				)
		}
	}

	*block = temp
}