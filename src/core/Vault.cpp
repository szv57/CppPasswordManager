#include "Vault.h"
#include "json.hpp"
#include <map>
#include <string>
#include <iostream>

using json = nlohmann::json;

Vault::Vault(const std::vector<unsigned char>& Nonce, const std::vector<unsigned char>& Salt,const json& Entries)
{
	this->Nonce = Nonce;
	this->Salt = Salt;
	if (Entries.is_array())
	{
		this->Entries = Entries;
	}
	else
	{
		this->Entries = json::array();
	}
}

void Vault::AddEntry(const std::string& Service, const std::string& Username, const std::vector<unsigned char>& Encrypted_Password)
{
	
	json Entry = 
	{
		{"service", Service},
		{"username", Username},
		{"password", Encrypted_Password }
	};

	try
	{
		this->Entries.push_back(Entry);
	}
	catch (const std::exception& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
		return;
	}

}
