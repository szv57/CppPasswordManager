#pragma once
#include "Vault.h"

#include <vector>
#include <string>




class CommandParser
{
public:

	static void ParseCommands(Vault& CurrentVault,const std::vector<unsigned char>& Key, const std::string& Input, const std::string& Path);

	static void ExecuteNew(Vault& CurrentVault, const std::vector<unsigned char>& Key);

	static void ExecuteGet(Vault& CurrentVault, const std::vector<unsigned char>& Key, const std::string& Service);

	static void ExecuteDelete(Vault& CurrentVault, const std::string& Service);

	static void ExecuteUpdate(Vault& CurrentVault, const std::vector<unsigned char>& Key, const std::string& Service);

	static void Help();

	static std::optional<Vault> RegisterSetup(const std::string& Path, const std::string& MasterPass);

	static std::vector<std::string> TokenizeCommandline(std::string CommandLine);

protected:
private:
};