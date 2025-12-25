#include <iostream>
#include <sodium.h>
#include <algorithm>
#include <fstream>
#include <stdexcept>

#include "Vault.h"
#include "CommandParser.h"
#include "Storage.h"
#include "Crypto.h"

int main() 
{
    if (sodium_init() < 0) {
        std::cerr << "ERROR: Failed to initialize Sodium." << std::endl;
        return 1;
    }

    std::cout << "Path: ";
    std::string Path;

    std::getline(std::cin, Path);

    std::cout << std::endl << std::endl;

    std::cout << "Login or Register(l/r)" << std::endl;

    std::string LogOrReg;

	do{
		if (LogOrReg != "")
		{
            std::cerr << "Invalid input. ('r' = Register | 'l' = Login)" << std::endl;
		}

        std::cout << ">";

        getline(std::cin, LogOrReg);

        for (auto& c : LogOrReg)
        {
            c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }

		
    } while (LogOrReg != "l" && LogOrReg != "r");
    

    if (LogOrReg == "r")
    {
        std::cout << "\nSet the master password: ";
        std::string MasterPassword;
        try
        {
            Storage::GenerateVaultFile(Path);
        }
        catch (const std::fstream::failure e)
        {
            std::cerr << "ERROR: " << e.what() << std::endl;
            return 1;
        }

        std::getline(std::cin, MasterPassword);

        auto tVault = CommandParser::RegisterSetup(Path, MasterPassword);

        if (!tVault.has_value())
        {
            std::cerr << "Failed to make the base vault." << std::endl;
            return 1;
        }

        Vault CurrentVault = tVault.value();

        std::vector<unsigned char> Key = Crypto::DeriveKey(MasterPassword, CurrentVault.Salt);

        std::cout << "\nWhat do you want to do? ";
        std::string Input;

        std::getline(std::cin, Input);

        std::cout << std::endl;

        
        while (true)
        {
            if (Input != "" && Input != "exit")
            {
                CommandParser::ParseCommands(CurrentVault, Key, Input, Path);

                std::cout << "\nWhat do you want to do? ";

                std::getline(std::cin, Input);

                std::cout << std::endl;
            }
            else
            {
                break;
            }
            
        }
       
        
    }
    else if(LogOrReg == "l")
    {
        std::cout << "What is the master password: ";
        std::string MasterPass;

        std::getline(std::cin, MasterPass);

        auto tCurrentVault = Storage::LoadDataFromJson(Path);

        if (!tCurrentVault.has_value())
        {
            std::cerr << "Could not load data from json file"; 
            return 1;
        }

        Vault CurrentVault = tCurrentVault.value();

        std::vector<unsigned char> Key = Crypto::DeriveKey(MasterPass, CurrentVault.Salt);
        try
        {
			if (!Crypto::VerifyMaster(CurrentVault.Sentinel, Key, CurrentVault.SentinelNonce))
			{
				std::cerr << "Invalid password. Access denied." << std::endl;
				return 0;
			}
        }
        catch (const std::runtime_error& e)
        {
            std::cerr << e.what() << std::endl;
            return 1;
        }
        

        std::cout << "Access granted!" << std::endl;

		std::cout << "\nWhat do you want to do? ";
		std::string Input;

		std::getline(std::cin, Input);

		std::cout << std::endl;

		while (true)
		{
			if (Input != "" && Input != "exit")
			{
				CommandParser::ParseCommands(CurrentVault, Key, Input, Path);

				std::cout << "\nWhat do you want to do? ";

				std::getline(std::cin, Input);

				std::cout << std::endl;
			}
            else 
            {
                break;

            }
			
		}

    }

    return 0;
}