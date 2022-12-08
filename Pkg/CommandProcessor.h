#pragma once
#include "Globals.h"
#include <vector>
class CommandProcessor
{
public:
	static void GetCommandAction(std::vector<std::string>& arguments);
};

