#pragma once
#include "Globals.h";

class ScaffoldHandler
{
public:
	static bool Install(std::string& scaffoldName);
	static size_t WriteCallback(char* ptr, size_t size, size_t nmemb, void* f);
	static bool ExtractAndRemove(std::string& zipPath);
	static void ListAvailableScaffolds(std::string& language);
};