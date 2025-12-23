#include "Storage.h"
#include <fstream>
#include <iostream>
#include <optional>

#include "json.hpp"



void Storage::SaveData(const json& JsonData, const std::string& path)
{

	if (path.find(".json") == std::string::npos)
	{
		std::cerr << "Invalid file type" << std::endl;
		return;
	}
	std::ofstream Outfile(path);
	
	if (Outfile.is_open())
	{
		Outfile << std::setw(4) << JsonData << std::endl;
		std::cout << "File successfully saved" << std::endl;
		return;
	}
	std::cerr << "Could not open file." << std::endl;
	
}

std::optional<json> Storage::LoadDataFromJson(const std::string& path)
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
		return Data;
	}
	std::cerr << "Could not open file." << std::endl;
	return std::nullopt;
}
