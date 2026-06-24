package main

import (
	"encoding/hex"
	"fmt"
	"os"

	"lox617app/lox617"
)

func keyTo32(
	key string,
) [32]byte {

	var out [32]byte

	bytes := []byte(key)

	for i := 0; i < 32; i++ {

		out[i] =
			bytes[
				i%len(bytes),
			]
	}

	return out
}

func textToBlock(
	text string,
) [32]byte {

	var block [32]byte

	bytes := []byte(text)

	length := len(bytes)

	if length > 32 {
		length = 32
	}

	copy(
		block[:],
		bytes[:length],
	)

	return block
}

func printHex(
	block *[32]byte,
) {

	fmt.Println(
		hex.EncodeToString(
			block[:],
		),
	)
}

func main() {

	if len(os.Args) < 6 {

		fmt.Println(
			"enc -key <key> -text <text>",
		)

		return
	}

	mode := os.Args[1]

	keyInput := os.Args[3]

	key :=
		keyTo32(
			keyInput,
		)

	if mode == "enc" {

		text := os.Args[5]

		block :=
			textToBlock(
				text,
			)

		lox617.Encrypt(
			&block,
			&key,
			16,
		)

		printHex(
			&block,
		)
	}
}