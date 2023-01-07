#pragma once
#include "Globals.h"
class Storage
{
	static bool directoryExists(std::string& path);
	static std::vector<std::string> filesInDirectory(std::string& path);
};

