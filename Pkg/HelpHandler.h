#pragma once
#include "Globals.h"
#include <vector>
class HelpHandler
{
public:
	static void RenderHelpPage();
	HelpHandler(std::string option);
private:
	std::vector<std::string> options;
	std::map<std::string, std::vector<std::string>> optionContent;
};

