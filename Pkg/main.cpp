#include <iostream>

#include "Globals.h"


using namespace std;


int main(int argc, char *argv[]) {
	if (argc == 0 || argc == 1)
	{
		cout << "please provide a command.." << endl;
		return 0;
	}
	vector<string> arguments;
	for (int i = 0; i < argc; i++)
	{
		arguments.push_back(argv[i]);
	}
	string value = "joe";
	if (in_array(value, arguments))
	{
		cout << "arguments has joe! :)";
	}
	CommandProcessor::GetCommandAction(arguments);
	return 0;
}






/*
* OUDE CODE, VERPLAATST NAAR "CommandProcessor::GetCommandAction(std::vector<std::string> arguments)"
* in CommandProcessor.h
using json = nlohmann::json;
curlpp::Cleanup myCleanup;

string packageList = Request::Make("http://packagemanager.chrisvanlier.nl/list.json");
json packages = json::parse(packageList);


cout << "All packages:" << endl;
for (auto it = packages.begin(); it != packages.end(); ++it)
{
	string name = (string)(*it)["language"] + "/" + (string)(*it)["name"];
	cout << name << endl;
}
*/