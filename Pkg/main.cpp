#include <iostream>

#include "Globals.h"


using namespace std;


int main(int argc, char *argv[]) {
	if (argc == 0 || argc == 1)
	{
		cout << RED << "Error: " << RESET << "please provide a command.." << endl;
		return 0;
	}
	vector<string> arguments;
	arguments.reserve(argc);
	for (int i = 0; i < argc; i++)
	{
		arguments.emplace_back(argv[i]);
	}
	CommandProcessor::GetCommandAction(arguments);
	return 0;
}

