#pragma once
#include "Globals.h"
class Request {
public:
	static std::string Make(std::string url);
	static bool DownloadZip(std::string url);
private:
	static bool init();
	static curlpp::Cleanup MyCleanup;
};