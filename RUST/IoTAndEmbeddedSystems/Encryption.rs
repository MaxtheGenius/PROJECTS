use aes::Aes128;
use aes::cipher::{BlockEncrypt, KeyInit};
use base64::encode;
use serde::Serialize;
use serde_json::to_string;

type Aes128Encryptor = aes::cipher::BlockEncryptor<Aes128>;

const ENCRYPTION_KEY: &[u8; 16] = b"securekey1234567";

pub fn encrypt_data<T: Serialize>(data: &T) -> Result<String, String> {
    // Serialize data to JSON
    let json_data = to_string(data).map_err(|e| format!("Serialization error: {}", e))?;

    // Initialize AES encryptor
    let encryptor = Aes128::new_from_slice(ENCRYPTION_KEY)
        .map_err(|e| format!("Key initialization error: {}", e))?;

    // Pad data to 16-byte block
    let mut padded_data = json_data.as_bytes().to_vec();
    while padded_data.len() % 16 != 0 {
        padded_data.push(0);
    }

    // Encrypt in 16-byte blocks
    let mut encrypted_data = Vec::new();
    for chunk in padded_data.chunks_exact(16) {
        let mut block = aes::Block::from_slice(chunk);
        encryptor.encrypt_block(&mut block);
        encrypted_data.extend_from_slice(&block);
    }

    // Base64 encode encrypted data
    Ok(encode(&encrypted_data))
}