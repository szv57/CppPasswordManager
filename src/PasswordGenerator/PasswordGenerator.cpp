#include "PasswordGenerator.h"
#include <string>
#include <random>
#include <iostream>


std::string PasswordGenerator::GeneratePass(const int& Length, const GeneratorSettings& Settings)
{
	std::string CharacterSet;
	if (Settings.Lower) CharacterSet += "abcdefghijklmnopqrstuvwxyz";
	if (Settings.Upper) CharacterSet += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	if (Settings.Digit) CharacterSet += "0123456789";
	if (Settings.Symbol) CharacterSet += "!@#$%^&*()-_=+";

	std::string Password;
	
	if (CharacterSet.empty())
	{
		CharacterSet = "abcdefghijklmnopqrstuvwxyz";
	}

	int RandomIndex;

	std::random_device Rand;

	std::mt19937 Gen(Rand());

	std::uniform_int_distribution<> Distribution(0, CharacterSet.length() - 1);

	for (int i = 0; i < Length; i++)
	{
		RandomIndex = Distribution(Gen);

		Password += CharacterSet[RandomIndex];
	}

	std::cout << "Password successfully generated: " << Password;

	return Password;
}
