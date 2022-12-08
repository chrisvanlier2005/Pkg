#pragma once
#include "Globals.h"
class Console
{
public:
	static void SetColor(std::string colorCode);
	static void Write(std::string text);
private:
	static std::string currentColor;
};

