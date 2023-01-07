#include "CommandProcessor.h"
#include "Globals.h"
#include "InstallationHandler.h"
using namespace std;
using json = nlohmann::json;

enum Options {
	initialize,
	install,
	help,
	update,
	listPackages,
	removePackage,
	noCommand,
	scaffold
};

Options HashIt(std::string const& inString) {
	if (inString == "install" || inString == "i") return install;
	if (inString == "help" || inString == "h") return help;
	if (inString == "update" || inString == "u") return update;
	if (inString == "list" || inString == "l") return listPackages;
	if (inString == "initialize" || inString == "init") return initialize;
	if (inString == "remove" || inString == "r") return removePackage;
	if (inString == "scaffold" || inString == "s") return scaffold;
	return noCommand;
}

void CommandProcessor::GetCommandAction(vector<string>& arguments) {
	string command  = arguments[1];
	switch (HashIt(command))
	{
		case initialize:
		{
			if (ConfigHandler::CreateConfigFile())
			{
				cout << "Config file created!" << endl;
			}
			else {
				cout << "PKGConfig.json not created" << endl;
			}
		}
		break;
		case install:
		{
			json config = ConfigHandler::GetConfigFile();

			// expects a second argument
			if (arguments.size() == 2)
			{
				cout << "Please provide a package name";
				return;
			}
			if (!ConfigHandler::ConfigExists) {
				cout << "No PKGConfig.json exists in this scope" << endl;
				return;
			}
			cout << "Proceeding installation of " << arguments[2] << endl;
			InstallationHandler installHandler{config};
			string packageComplete = arguments[2];
			// split the package name and language from eachother
			string language = packageComplete.substr(0, packageComplete.find('/'));
			string packageName = packageComplete.substr(packageComplete.find('/') + 1);
			bool result = installHandler.installPackage(language, packageName);
			if (!result) {
				cout << RED << "Package not installed" << RESET << endl;
			}
			else {
				cout << BOLDWHITE << "Cleaning up..." << RESET << endl;
			}
		}
		break;
		case removePackage:
		{
			json config = ConfigHandler::GetConfigFile();
			if (arguments.size() == 2)
			{
				cout << RED << "Please provide a Package name." << RESET << endl;
			}
			if (!ConfigHandler::ConfigExists)
			{
				cout << RED << "Config file does not exist." << RESET << endl;
				return;
			}
			InstallationHandler installHandler{ config };
			string packageComplete = arguments[2];
			string language = packageComplete.substr(0, packageComplete.find('/'));
			string packageName = packageComplete.substr(packageComplete.find('/') + 1);
			bool result = installHandler.removePackage(language, packageName);
			if (!result) {
				cout << RED << "Package " << packageComplete << "Not removed" << RESET << endl;
			}
			else {

			}
		}
		case help: 
		{
			if (arguments.size() == 2)
			{
				cout << "Commands:" << endl;
				cout <<
					"  - initialize \n" <<
					"  - install (i) | PackageName\n" <<
					"  - help (h) | CommandForHelp\n" <<
					"  - update (u) | PackageName / empty\n" <<
					"  - list (l) | empty / page\n"
					<< endl;
				return;
			}
			switch (HashIt(arguments[2]))
			{
				case install:
				{

				}
				break;
				default:	
					break;
			}
		}
		break;
		case update:
		{

		}
		break;
		case listPackages:
		{
			curlpp::Cleanup myCleanup;

			string packageList = Request::Make("http://packagemanager.chrisvanlier.nl/list.php");
			json packages = json::parse(packageList);
			json firstPackage = packages[0];

			cout << "All packages:" << endl;
			for (auto it = packages.begin(); it != packages.end(); ++it)
			{
				try
				{
					string name = (string)(*it)["language"] + '/' + (string)(*it)["packageName"];
					cout << name << endl;
					name.clear();
				}
				catch (const json::exception exep)
				{
					std::cout << exep.what() << endl;
				}

			}
		}
		break;
		case scaffold:
		{
			if (arguments.size() == 2)
			{
				cout << YELLOW << "Please provide a scaffold name" << RESET;
				exit(0);
			}
			string scaffoldAction = arguments[2];
			string scaffoldName = arguments[2];
			string language = scaffoldName.substr(0, scaffoldName.find('/'));
			if (scaffoldAction == "list" || scaffoldAction == "l")
			{
				if (arguments.size() != 4)
				{
					cout << RED << "please provide a language to list" << RESET << endl;
					exit(0);
				}
				language = arguments[3];
				ScaffoldHandler::ListAvailableScaffolds(language);
			} else {
				ScaffoldHandler::Install(scaffoldAction);
			}
		}
		break;
		case noCommand:
		{
			// set color to red
			cout << "\033[1;31mCommand does not exist, use command help / h for a list of commands.\033[0m" << endl;
			
			exit(0);
		}
		default:
		{

		}
		break;
	}
}