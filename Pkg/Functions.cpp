#include "Globals.h"
#include <direct.h>
using namespace std;

std::string currentPath() {
	char buff[FILENAME_MAX]; // een string om het huidige path bij te houden
	_getcwd(buff, FILENAME_MAX);
	std::string currentWorkingDirectory(buff);
	return currentWorkingDirectory;
}
bool in_array(const std::string &value, const std::vector<std::string> &array) {
	return std::find(array.begin(), array.end(), value) != array.end();
}
/// <summary>
///		Split a string to a vector the size of 2, split it at character.
/// </summary>
/// <param name="value"></param>
/// <param name="character"></param>
/// <returns></returns>
std::vector<std::string> split(const std::string& value, const char character) {
	return std::vector<std::string> {"joe"};
}

bool request_confirmation(std::string text) {
	std::cout << text << " (y/n)" << std::endl;
	std::string answer;
	std::cin >> answer;
	while (answer != "y" && answer != "n") {
		std::cin.clear();
		std::cin.ignore(10000, '\n');
		std::cout << answer << " is an invalid answer" << std::endl;
		std::cout << text << "(y/n)" << std::endl;
	}
	if (answer == "y")
	{
		return true;
	}
	return false;
}


bool replace(std::string& str, const std::string& from, const std::string& to) {
	size_t start_pos = str.find(from);
	if (start_pos == std::string::npos)
		return false;
	str.replace(start_pos, from.length(), to);
	return true;
}

void error(const string& error_message)
{
	cout << RED << "Error: " << RESET << error_message << endl;
	exit(0);
}