#include "CommandParser.h"
#include "Crypto.h"
#include "Vault.h"
#include "Storage.h"
#include "json.hpp"
#include "PasswordGenerator.h"
#include "sodium.h"

#include <sstream>
#include <iostream>
#include <string>
#include <fstream>

void CommandParser::ParseCommands(Vault& CurrentVault, const std::vector<unsigned char>& Key, const std::string& Input, const std::string& Path)
{
	std::vector<std::string> Tokens = TokenizeCommandline(Input);

	std::map<std::string, int> TokenMap =
	{
		{"new", 0},
		{"get" , 1},
		{"delete", 2},
		{"update", 3},
		{"help", 4},
		{"exit", 5},
		{"save", 6}
	};


	if (Tokens.empty())
	{
		std::cerr << "ERROR: No commands given." << std::endl;
		return;
	}

	std::string Command = Tokens[0];
	int CommandInt;
	try
	{
		CommandInt = TokenMap.at(Command);

	}
	catch (const std::exception&)
	{
		std::cerr << Command << " is not a valid command. " << std::endl;
		return;
	}
	 

	switch (CommandInt)
	{
	case 0:
		ExecuteNew(CurrentVault, Key);
		break;

	case 1:
		if (Tokens.size() != 2)
		{
			std::cerr << "Invalid amount of arguments for command 'get'" << std::endl;
			break;
		}
		ExecuteGet(CurrentVault, Key, Tokens[1]);
		break;

	case 2:
		if (Tokens.size() != 2)
		{
			std::cerr << "Invalid amount of arguments for command 'delete'" << std::endl;
			break;
		}
		ExecuteDelete(CurrentVault, Tokens[1]);
		break;
	case 3:
		if (Tokens.size() != 2)
		{
			std::cerr << "Invalid amount of arguments for command 'update'" << std::endl;
			break;
		}
		ExecuteUpdate(CurrentVault, Key, Tokens[1]);
		break;

	case 4:
		Help();
		break;

	case 5:
		break;

	case 6:
		Storage::SaveData(CurrentVault, Path);
		break;
	default:
		break;
	}

}

void CommandParser::ExecuteNew(Vault& CurrentVault, const std::vector<unsigned char>& Key)
{
	std::cout << "Service: ";
	std::string Service;

	std::cout << std::endl;

	std::getline(std::cin, Service);

	std::cout << "Username: ";
	std::string Username;

	std::cout << std::endl;

	std::getline(std::cin, Username);



	std::cout << "Password ";


	std::vector<unsigned char> Nonce;

	std::string StringPass;


	Nonce = Crypto::GenerateNonce();

	

	std::getline(std::cin, StringPass);

	if (StringPass == "" || StringPass == "\n")
	{
		StringPass = PasswordParse("gen 16");
	}
	else
	{
		StringPass = PasswordParse(StringPass);
	}

	if (StringPass == "")
	{
		std::cerr << "Invalid password generation parameters. Password is auto generated. Update with keyword 'update'" << std::endl;
		StringPass = PasswordParse("gen 20");
	}

	std::cout << std::endl;

	auto tEntry = Entry::FromStringPass(Service, Username, StringPass, Nonce, Key);

	sodium_memzero(StringPass.data(), StringPass.size());
	StringPass.clear();
	StringPass.shrink_to_fit();
	
	if (tEntry.has_value())
	{
		Entry NewEntry = tEntry.value();

		CurrentVault.AddEntry(NewEntry);

		return;
	}

	std::cerr << "Password Encryption failed." << std::endl;

}

void CommandParser::ExecuteGet(Vault& CurrentVault, const std::vector<unsigned char>& Key, const std::string& Service)
{
	auto TempEntry = CurrentVault.GetEntry(Service);
	if (TempEntry.has_value())
	{
		Entry ReturnedEntry = TempEntry.value();

		auto tDecryptedPass = Crypto::DecryptPassword(ReturnedEntry.Encrypted_Password, Key, ReturnedEntry.Nonce);
		
		if (tDecryptedPass.has_value())
		{
			std::string DecryptedPassword = tDecryptedPass.value();

			std::cout << "Service: " << ReturnedEntry.Service << std::endl;
			std::cout << "Username: " << ReturnedEntry.Username << std::endl;
			std::cout << "Password: " << DecryptedPassword << std::endl;

			sodium_memzero(DecryptedPassword.data(), DecryptedPassword.size());
			DecryptedPassword.clear();
			DecryptedPassword.shrink_to_fit();

			return;
		
		}
		
		std::cerr << "Decrypting password failed." << std::endl;
		return;
		
	}
	std::cerr << "Retrieving entry failed. " << std::endl;

}

void CommandParser::ExecuteDelete(Vault& CurrentVault, const std::string& Service)
{
	try
	{
		CurrentVault.DeleteEntry(Service);

		std::cout << "\n" << Service << "successfully deleted" << std::endl;
	}
	catch (const std::invalid_argument& e)
	{
		std::cerr << "ERROR: " << e.what() << std::endl;
	}
}

void CommandParser::ExecuteUpdate(Vault& CurrentVault, const std::vector<unsigned char>& Key, const std::string& Service)
{
	std::cout << "New Username: ";
	std::string Username;

	std::getline(std::cin, Username);

	std::cout << std::endl;

	std::cout << "New Password: ";
	std::string Password;

	std::getline(std::cin, Password);

	if (Password == "" || Password == "\n")
	{
		Password = PasswordParse("gen 20");
	}
	else
	{
		Password = PasswordParse(Password);
	}

	if (Password == "")
	{
		std::cerr << "Invalid password generation parameters. Password is auto generated. Update with keyword 'update'" << std::endl;
		Password = PasswordParse("gen 20");
	}
	std::cout << std::endl;

	std::vector<unsigned char> Nonce = Crypto::GenerateNonce();

	auto tEntry = Entry::FromStringPass(Service, Username, Password, Nonce, Key);

	sodium_memzero(Password.data(), Password.size());
	Password.clear();
	Password.shrink_to_fit();

	if (tEntry.has_value())
	{

		Entry UpdatedEntry = tEntry.value();
		CurrentVault.UpdateEntry(UpdatedEntry);
		return;
	}
	std::cerr << "Failed to update entry" << std::endl;
	

}

std::string CommandParser::ExecutePasswordGeneration(std::vector<std::string> Tokens, const int& Length)
{
	GeneratorSettings Settings;
	Settings.Upper = true;
	Settings.Lower = true;
	Settings.Digit = true;
	Settings.Symbol = true;

	for (auto& Token : Tokens)
	{
		if (Token == "symbol")
		{
			Settings.Symbol = false;
		}
		if (Token == "digit")
		{
			Settings.Digit = false;
		}
		if (Token == "upper")
		{
			Settings.Upper = false;
		}
		if (Token == "lower")
		{
			Settings.Lower = false;
		}
	}
	std::string Generated = PasswordGenerator::GeneratePass(Length, Settings);

	return Generated;
}

std::string CommandParser::PasswordParse(std::string Input)
{
	std::vector<std::string> Tokens = TokenizeCommandline(Input);

	if (Tokens.size() == 1)
	{
		return Tokens[0];
	}


	if (Tokens[0] != "gen" || Tokens.size() == 3 || Tokens.size() >= 7)
	{
		return "";
	}

	std::stringstream SS(Tokens[1]);


	int Length;

	try
	{
		SS >> Length;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return "";
	}

	if (Tokens.size() == 2)
	{
		std::string Pass = ExecutePasswordGeneration(Tokens, Length);
		return Pass;
	}

	if (Tokens[2] != "--no")
	{
		return "";
	}

	std::string Pass = ExecutePasswordGeneration(Tokens, Length);
	return Pass;
}

void CommandParser::Help()
{
	std::cout << "\nHELP" << std::endl;
	std::cout << "new: Create a new entry in the vault" << std::endl;
	std::cout << "get <service>: Get an entry from the vault by service." << std::endl;
	std::cout << "delete <service>: Delete an entry from the vault by service." << std::endl;
	std::cout << "update <service>: Update entry by service." << std::endl;
	std::cout << "help: Show help." << std::endl;
	std::cout << "exit: Exit program.\n" << std::endl;
}

std::optional<Vault> CommandParser::RegisterSetup(const std::string& Path, const std::string& MasterPass)
{
	

	std::vector<unsigned char> Salt = Crypto::GenerateSalt();

	std::vector<unsigned char> Nonce = Crypto::GenerateNonce();

	std::vector<unsigned char> Key = Crypto::DeriveKey(MasterPass, Salt);

	auto tSentinel = Crypto::GenerateSentinel(Key, Nonce);

	if (tSentinel.has_value())
	{
		std::vector<unsigned char> Sentinel = tSentinel.value();

		Vault ReturnVault(Nonce, Salt, Sentinel, {});

		Storage::SaveData(ReturnVault, Path);

		return ReturnVault;
	}

	return std::nullopt;

}

std::vector<std::string> CommandParser::TokenizeCommandline(std::string CommandLine)
{
	std::istringstream ISS(CommandLine);
	std::vector<std::string> Tokens;

	std::string Token;

	while (ISS >> Token)
	{
		Tokens.push_back(Token);
	}

	return Tokens;
}
