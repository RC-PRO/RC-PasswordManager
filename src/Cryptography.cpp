#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <openssl/sha.h>
#include <openssl/bio.h>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/buffer.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "User32.lib")

#include "PasswordAndKey.h"
#include "UUID.h"

std::string generate_salt() {
    size_t length = 16;
    unsigned char salt[32];
    RAND_bytes(salt, length);

    std::ostringstream oss;
    for (size_t i = 0; i < length; i++) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)salt[i];
    }
    return oss.str();

}

std::string sha256(const std::string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)input.c_str(), input.length(), hash);

    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return oss.str();
}

std::string base64_encode(const std::string& input) {
    BIO* bio;
    BIO* b64;

    BUF_MEM* buffer;

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bio);

    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);

    BIO_write(b64, input.data(), input.size());
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &buffer);

    std::string result(buffer->data, buffer->length);
    BIO_free_all(b64);

    return result;
}

std::string base64_decode(const std::string& input) {
    BIO* bio;
    BIO* b64;

    int decode_length = input.length();
    
    std::string output;
    output.resize(decode_length);

    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf(input.data(), input.length());
    bio = BIO_push(b64, bio);

    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);

    int decoded_length = BIO_read(bio, output.data(), output.length());
    
    if (decoded_length <= 0) {
        BIO_free_all(bio);
    }

    output.resize(decoded_length);
    BIO_free_all(bio);

    return output;
}

std::string AESKey(const std::string& pwhash) {
    std::string uuid = GetUUID();
    std::string combined = uuid + pwhash;

    std::string hash = sha256(combined);

    std::string key;
    for (size_t i = 0; i < hash.length(); i += 2) {
        std::string byte_string = hash.substr(i, 2);
        char byte = static_cast<char>(std::stoi(byte_string, nullptr, 16));
        key += byte;
    }
    return key;
}

std::string aes_e(const std::string text, const std::string& key) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    unsigned char iv[16];
    RAND_bytes(iv, sizeof(iv));

    std::string encrypted;
    encrypted.resize(text.size() + AES_BLOCK_SIZE);

    int length;
    int encrypted_length;

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (const unsigned char*)key.data(), iv);
    EVP_EncryptUpdate(ctx, (unsigned char*)&encrypted[0], &length, (const unsigned char*)text.data(), text.size());
    encrypted_length = length;
    EVP_EncryptFinal_ex(ctx, (unsigned char*)&encrypted[0] + length, &length);
    encrypted_length += length;

    EVP_CIPHER_CTX_free(ctx);
    encrypted.resize(encrypted_length);

    std::string output(reinterpret_cast<char*>(iv), sizeof(iv));
    output += encrypted;

    return output;

}

std::string aes_d(const std::string encrypted, const std::string& key) {

    if (encrypted.size() < 16) {
        return "";
    }

    const unsigned char* iv = (const unsigned char*)encrypted.data();
    const unsigned char* p_encrypted = (const unsigned char*)encrypted.data() + 16;
    int encrypted_length = encrypted.size() - 16;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();

    std::string decrypted;
    decrypted.resize(encrypted_length);

    int length;
    int decrypted_length;

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (const unsigned char*)key.data(), iv);
    EVP_DecryptUpdate(ctx, (unsigned char*)&decrypted[0], &length, p_encrypted, encrypted_length);
    decrypted_length = length;
    EVP_DecryptFinal_ex(ctx, (unsigned char*)&decrypted[0] + length, &length);
    decrypted_length += length;

    EVP_CIPHER_CTX_free(ctx);
    decrypted.resize(decrypted_length);
    return decrypted;
}