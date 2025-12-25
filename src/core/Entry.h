#pragma once
#include <string>
#include <vector>
#include <optional>

class Entry
{
public:

	Entry(const std::string& Service, const std::string& Username, std::vector<unsigned char> Encrypted_Password, const std::vector<unsigned char> Nonce);

	static std::optional<Entry> FromStringPass(const std::string& Service, const std::string Username, const std::string& Password, const std::vector<unsigned char>& Nonce, std::vector<unsigned char> Key);

	std::string Service;
	std::string Username;
	std::vector<unsigned char> Encrypted_Password;
	std::vector<unsigned char> Nonce;

};