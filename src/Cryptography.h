#include <iostream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <openssl/sha.h>
#include <openssl/rand.h>

std::string generate_salt();

std::string sha256(const std::string& input);

std::string base64_encode(const std::string& input);

std::string base64_decode(const std::string& input);

std::string AESKey(const std::string& pwhash);

std::string aes_e(const std::string text, const std::string& key);

std::string aes_d(const std::string encoded, const std::string& key);