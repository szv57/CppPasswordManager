#include "Vault.h"
#include "json.hpp"
#include "Entry.h"
#include <map>
#include <string>
#include <iostream>
#include <stdexcept>

using json = nlohmann::json;

Vault::Vault(const std::vector<unsigned char>& SentinelNonce, const std::vector<unsigned char>& Salt, const std::vector<unsigned char>& Sentinel, const json& Entries)
{
	this->SentinelNonce = SentinelNonce;
	this->Salt = Salt;
	this->Sentinel = Sentinel;
	if (Entries.is_array())
	{
		this->Entries = Entries;
	}
	else
	{
		this->Entries = json::array();
	}
}

void Vault::AddEntry(const Entry& EntryToAdd)
{
	
	json Entry = 
	{
		{"service", EntryToAdd.Service},
		{"username", EntryToAdd.Username},
		{"password", EntryToAdd.Encrypted_Password},
		{"nonce", EntryToAdd.Nonce}
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

void Vault::DeleteEntry(const std::string& Service)
{
	if (!Entries.is_array()) {
		throw std::runtime_error("Vault entries are not properly initialized.");
	}

	for (auto it = Entries.begin(); it != Entries.end(); ++it) {
		if ((*it)["service"] == Service) {
			Entries.erase(it); 
			return;
		}
	}
	throw std::invalid_argument("Entry not in vault");
}

std::optional<Entry> Vault::GetEntry(const std::string& Service)
{

	for (auto e : this->Entries)
	{
		if (e["service"] == Service)
		{
			std::string User = e["username"];
			std::string GetService = e["service"];
			auto Pass = e["password"].get<std::vector<unsigned char>>();
			auto Nonce = e["nonce"].get<std::vector<unsigned char>>();

			Entry ReturnEntry(GetService, User, Pass, Nonce);

			return ReturnEntry;
		}
	}
	return std::nullopt;
}

void Vault::UpdateEntry(const Entry& UpdatedEntry)
{
	for(auto& e : this->Entries)
	{ 
		if (e["service"] == UpdatedEntry.Service)
		{
			e["username"] = UpdatedEntry.Username;
			e["password"] = UpdatedEntry.Encrypted_Password;
			return;
		}
			
	}
	std::cerr << UpdatedEntry.Service << " not found in the vault." <<std::endl;
}

