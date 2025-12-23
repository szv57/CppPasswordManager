#pragma once
#include <optional>
#include <string>
#include <vector>
#include "sodium.h"



class Crypto
{
public:

	static std::optional<std::vector<unsigned char>> EncryptPassword(const std::string& Password, const std::vector<unsigned char> Key, const std::vector<unsigned char>& Nonce);

	static std::optional<std::string> DecryptPassword(const std::vector<unsigned char>& EncryptedPassword, const std::vector<unsigned char>& Key, const std::vector<unsigned char>& Nonce);

	static std::vector<unsigned char> DeriveKey(const std::string& MasterPass, const std::vector<unsigned char>& Salt);

	static std::vector<unsigned char> GenerateNonce();

	static std::vector<unsigned char> GenerateSalt();


protected:
private:
};