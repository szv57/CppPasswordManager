#pragma once
#include <optional>
#include <string>
#include <vector>
#include <map>

#include "Entry.h"
#include "json.hpp"

using json = nlohmann::json;

class Vault
{
public:

	Vault(const std::vector<unsigned char>& SentinelNonce,const std::vector<unsigned char>& Salt, const std::vector<unsigned char>& Sentinel, const json& Entries);

	std::vector<unsigned char> SentinelNonce;

	std::vector<unsigned char> Salt;

	std::vector<unsigned char> Sentinel;

	json Entries;

	void AddEntry(const Entry& EntryToAdd);

	void DeleteEntry(const std::string& Service);

	std::optional<Entry> GetEntry(const std::string& Service);

	void UpdateEntry(const Entry& UpdatedEntry);
	
protected:
private:

	
};