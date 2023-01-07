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
	run,
	noCommand,
	scaffold
};



/*
 * Todo: find better solution, 
 */
Options HashIt(string const& string_input) {
	if (string_input == "install" || string_input == "i") return install;
	else if (string_input == "remove" || string_input == "rm") return removePackage;
	else if (string_input == "run" || string_input == "r") return run;
	else if (string_input == "update" || string_input == "u") return update;
	else if (string_input == "list" || string_input == "l") return listPackages;
	else if (string_input == "initialize" || string_input == "init") return initialize;
	else if (string_input == "scaffold" || string_input == "s") return scaffold;
	else if (string_input == "help" || string_input == "h") return help;
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

			if (arguments.size() == 2)
			{
				cout << RED << "Error: " << RESET << "Please provide a package name\n";
				return;
			}
			if (!ConfigHandler::ConfigExists) {
				cout << "No PKGConfig.json exists in this scope\n";
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
			cout << RED << "Error:" << RESET << "This function has not been implemented as of yet.\n";
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
	case run:
		{
			json config = ConfigHandler::GetConfigFile();
			if (arguments.size() < 3)
			{
				string message = "Expected 3 arguments, got: " + to_string(arguments.size());
				cout << message << endl;
				error(message);
			}
			string scriptName = arguments[2];
			if(!RunScript::validate(scriptName))
			{
				error("Script '" + scriptName + "' Does not exist");
			}
			if (RunScript::run(scriptName))
			{
				cout << GREEN << "Success! " << RESET << "Script executed successfully\n";
			}
			else
			{
				error("Something went wrong while executing");
			}
		}
		break;
		case noCommand:
		{
			exit(0);
		}
		default:
		{

		}
		break;
	}
}