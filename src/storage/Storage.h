#pragma once
#include "json.hpp"
#include "Vault.h"
#include <fstream>
#include <optional>

using json = nlohmann::json;

class Storage
{
public:

	static void SaveData(const Vault& CurrentVault, const std::string& path);

	static std::optional<Vault> LoadDataFromJson(const std::string& path);

	static void GenerateVaultFile(const std::string& Path);

};