#pragma once
#include "Globals.h"
class InstallationHandler
{
public:
	InstallationHandler(nlohmann::json& configuration);
	bool installPackage(std::string& language, std::string& packageName);
	bool removePackage(std::string& language, std::string& packageName);
	bool serverPackageList();
	bool DownloadPackage(std::string& packageName);
	bool hasPackage(std::string& packageName);

private:
	std::vector<std::string> installedPackages;
	nlohmann::json& configuration;
};

