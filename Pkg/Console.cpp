#include "Console.h"
#include "Globals.h"

std::string Console::currentColor = WHITE;

void Console::SetColor(std::string colorCode) {
	Console::currentColor = colorCode;
	colorCode.clear();
}
void Console::Write(std::string text) {
	std::cout << Console::currentColor << text << RESET << std::endl;
	text.clear();
}

void Console::Serperator(int length = 10, char character = '-')
{
	for (size_t i = 0; i < length; i++)
	{
		std::cout << character;
	}
	std::cout << '\n';
}
