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