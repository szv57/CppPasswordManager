#pragma once
#include "string"
#include <vector>

struct Entry
{

	std::string Service;
	std::string Username;
	std::vector<unsigned char> Encrypted_Password;
	std::vector<unsigned char> Nonce

};