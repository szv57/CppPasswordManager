#include "Storage.h"
#include <fstream>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <map>

#include "json.hpp"
#include "Vault.h"

using json = nlohmann::json;

void Storage::SaveData(const Vault& CurrentVault, const std::string& path)
{

	if (path.find(".json") == std::string::npos)
	{
		std::cerr << "Invalid file type" << std::endl;
		return;
	}
	std::ofstream Outfile(path);

	json JsonData =
	{
		{"salt", CurrentVault.Salt},
		{"nonce", CurrentVault.SentinelNonce},
		{"sentinel", CurrentVault.Sentinel},
		{"entries", CurrentVault.Entries}
	};
	
	if (Outfile.is_open())
	{
		Outfile << std::setw(4) << JsonData << std::endl;
		std::cout << "File successfully saved" << std::endl;

		Outfile.close();
		return;
	}
	std::cerr << "Could not open file." << std::endl;
	
}

std::optional<Vault> Storage::LoadDataFromJson(const std::string& path)
{
	if (path.find(".json") == std::string::npos)
	{
		std::cerr << "Invalid file type" << std::endl;
		return std::nullopt;
	}
	std::ifstream Infile(path);

	if (Infile.is_open())
	{
		json Data = json::parse(Infile);

		if (!Data.contains("salt") || !Data.contains("nonce") || !Data.contains("entries"))
		{
			std::cerr << "File does not contain the required data" << std::endl;
			return std::nullopt;
		}

		std::vector<unsigned char> Salt;
		std::vector<unsigned char> Nonce;
		std::vector<unsigned char> Sentinel;
		json Entries;

		try
		{
			Salt = Data["salt"].get<std::vector<unsigned char>>();
			Nonce = Data["nonce"].get<std::vector<unsigned char>>();
			Sentinel = Data["sentinel"].get<std::vector<unsigned char>>();
			Entries = Data["entries"];


		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			return std::nullopt;
		}
		
		Vault ReturnVault(Nonce, Salt, Sentinel, Entries);

		return ReturnVault;
	}
	std::cerr << "Could not open file." << std::endl;
	return std::nullopt;
}

void Storage::GenerateVaultFile(const std::string& Path)
{
	std::fstream::failure e("Could not open file.");
	std::fstream::failure jsone("Path does not point to a json file");
	if (Path.find(".json") == std::string::npos)
	{
		throw jsone;
	}

	json BaseData =
	{
		{"salt", json::array()},
		{"nonce", json::array()},
		{"sentinel", json::array()},
		{"entries", json::array()}
	};

	std::ofstream OutFile(Path);

	if (OutFile.is_open())
	{
		OutFile << std::setw(4) << BaseData << std::endl;
		return;
	}

	throw e;
}
