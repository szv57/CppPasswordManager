#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include "Crypto.h"
#include "sodium.h"

void test_encryption_cycle() {
	std::cout << "Running: test_encryption_cycle..." << std::endl;

	std::string original = "Secret123";

	std::string master = "Master123";

	auto salt = Crypto::GenerateSalt();
	auto nonce = Crypto::GenerateNonce();

	auto Key = Crypto::DeriveKey(master, salt);

	auto encrypted = Crypto::EncryptPassword(original, Key, nonce);
	assert(encrypted.has_value() && "Encryption failed!");

	auto decrypted = Crypto::DecryptPassword(*encrypted, Key, nonce);
	assert(decrypted.has_value() && "Decryption failed!");
	assert(*decrypted == original && "Decrypted text does not match original!");

	std::cout << "SUCCESS: test_encryption_cycle passed!" << std::endl;
}

int main() {
	if (sodium_init() != 0) {
		std::cerr << "Failed to initialize libsodium!" << std::endl;
		return 1;
	}

	try {
		test_encryption_cycle();

		std::cout << "\nALL TESTS PASSED!" << std::endl;
	}
	catch (const std::exception& e) {
		std::cerr << "Test failed with exception: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}