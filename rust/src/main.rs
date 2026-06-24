use std::env;

use lox617::feistel::{
    lox617_encrypt,
    lox617_decrypt
};

use lox617::keyschedule::
    lox617_expand_key;

fn key_to_32(
    key: &str
) -> [u8; 32] {

    let mut out =
        [0u8; 32];

    let bytes =
        key.as_bytes();

    for i in 0..32 {

        out[i] =
            bytes[
                i % bytes.len()
            ];
    }

    out
}

fn text_to_block(
    text: &str
) -> [u8; 32] {

    let mut block =
        [0u8; 32];

    let bytes =
        text.as_bytes();

    let len =
        bytes.len().min(32);

    block[..len]
        .copy_from_slice(
            &bytes[..len]
        );

    block
}

fn hex_to_block(
    hex: &str
) -> [u8; 32] {

    let mut out =
        [0u8; 32];

    for i in 0..32 {

        let idx = i * 2;

        let byte =
            u8::from_str_radix(
                &hex[idx..idx+2],
                16
            ).unwrap();

        out[i] = byte;
    }

    out
}

fn print_hex(
    block: &[u8; 32]
) {

    for b in block {
        print!("{:02X}", b);
    }

    println!();
}

fn main() {

    let args:
        Vec<String> =
        env::args().collect();

    if args.len() < 5 {

        println!(
            "Usage:"
        );

        println!(
            "enc -key <key> -text <text>"
        );

        println!(
            "dec -key <key> -hex <hex>"
        );

        return;
    }

    let mode =
        &args[1];

    let key_input =
        &args[3];

    if key_input.len() < 15 {

        println!(
            "Key minimum 15 chars"
        );

        return;
    }

    let key =
        key_to_32(
            key_input
        );

    let subkeys =
    lox617_expand_key(
            &key,
            16
        );

    if mode == "enc" {

        let text =
            &args[5];

        let mut block =
            text_to_block(
                text
            );

        lox617_encrypt(
            &mut block,
            &subkeys,
            16
        );

        print_hex(
            &block
        );

    } else if mode == "dec" {

        let hex =
            &args[5];

        let mut block =
            hex_to_block(
                hex
            );

        lox617_decrypt(
            &mut block,
            &subkeys,
            16
        );

        let text =
            String::from_utf8_lossy(
                &block
            );

        println!(
            "{}",
            text
        );
    }
}