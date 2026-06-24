package lox617

import (
	"crypto/sha256"
)

func IntegritySeal(
	data []byte,
) [32]byte {

	return sha256.Sum256(
		data,
	)
}