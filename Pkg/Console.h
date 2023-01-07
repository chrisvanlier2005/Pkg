#pragma once
#include "Globals.h"
class Console
{
public:
	static void SetColor(std::string colorCode);
	static void Write(std::string text);
	static void Serperator(int length = 20, char character = '-');
private:
	static std::string currentColor;
};

