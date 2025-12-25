#include "Entry.h"
#include "Crypto.h"
#include <vector>
#include <string>
#include <optional>

Entry::Entry(const std::string& Service, const std::string& Username, std::vector<unsigned char> Encrypted_Password, const std::vector<unsigned char> Nonce)
{
	this->Service = Service;
	this->Encrypted_Password = Encrypted_Password;
	this->Nonce = Nonce;
	this->Username = Username;
}

std::optional<Entry> Entry::FromStringPass(const std::string& Service, const std::string Username, const std::string& Password, const std::vector<unsigned char>& Nonce, std::vector<unsigned char> Key)
{
	auto TempPass = Crypto::EncryptPassword(Password, Key, Nonce);

	if (TempPass.has_value())
	{
		std::vector<unsigned char> EnryptedPassword = TempPass.value();

		Entry ReturnEntry(Service, Username, EnryptedPassword, Nonce);

		return ReturnEntry;
	}

	return std::nullopt;
}
