#pragma once
#include <optional>
#include <string>
#include <vector>
#include <map>
#include "json.hpp"

class Vault
{
public:

	Vault(const std::vector<unsigned char>& Nonce,const std::vector<unsigned char>& Salt, const json& Entries);

	std::vector<unsigned char> Nonce;

	std::vector<unsigned char> Salt;

	json Entries;

	void AddEntry(const std::string& Service, const std::string& Username, const std::vector<unsigned char>& Encrypted_Password);

	void DeleteEntry(const std::string& Service);

	void GetEntry(const std::string& Service);
	
protected:
private:

	
};