package lox617

func lox617CatMap(
	block *[8]byte,
) {

	var temp [8]byte

	for i := 0; i < 8; i++ {

		for j := 0; j < 8; j++ {

			bit :=
				(block[i] >> j) & 1

			newX :=
				(2*i + j) % 8

			newY :=
				(i + j) % 8

			temp[newX] |=
				bit << newY
		}
	}

	*block = temp
}