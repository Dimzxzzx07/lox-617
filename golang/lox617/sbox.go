package lox617

func gfMul(a byte, b byte) byte {

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

func gfInv(a byte) byte {

	if a == 0 {
		return 0
	}

	var t byte = 1

	for i := 0; i < 254; i++ {
		t = gfMul(t, a)
	}

	return t
}

func generateSBox() [256]byte {

	var sbox [256]byte

	for i := 0; i < 256; i++ {

		x := gfInv(byte(i))

		y :=
			x ^
				((x << 1) | (x >> 7)) ^
				((x << 2) | (x >> 6)) ^
				0x63

		sbox[i] = y
	}

	return sbox
}