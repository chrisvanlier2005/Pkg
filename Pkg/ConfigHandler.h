#pragma once
#include "Globals.h"
#include "nlohmann/json.hpp"
class ConfigHandler
{
public:
	static bool CreateConfigFile();
	static nlohmann::json GetConfigFile();
	static bool WriteToConfig(nlohmann::json& jsonToWrite);
	static bool ConfigExists();
	bool HasDependency(nlohmann::json& configFile, std::string& dependencyName);
private:
	static std::string configLocation;

};

