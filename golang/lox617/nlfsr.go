package lox617

func entropyFold(
	state *uint64,
) {

	fb :=
		((*state >> 63) & 1) ^
			((*state >> 61) & 1) ^
			((*state >> 59) & 1) ^
			(((*state >> 55) & 1) &
				((*state >> 47) & 1)) ^
			(((*state >> 31) & 1) |
				((*state >> 23) & 1))

	fb &= 1

	*state =
		((*state << 1) |
			fb) &
			0xFFFFFFFFFFFFFFFF
}