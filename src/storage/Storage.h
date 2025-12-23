#pragma once
#include "json.hpp"
#include <fstream>

using json = nlohmann::json;

class Storage
{
public:

	static void SaveData(const json& JsonData, const std::string& path);

	static std::optional<json> LoadDataFromJson(const std::string& path);


};