#include "ConfigHandler.h"
#include "Globals.h"
#include <fstream>
using namespace std;
using json = nlohmann::json;




bool ConfigHandler::CreateConfigFile() {
	cout << "Checking if PKGConfig.json exists" << endl;
	if (ConfigHandler::ConfigExists())
	{
		cout << "Config file already exists." << endl;
		return false;
	}
	std::cout << "No config file found proceeding creation" << endl;
	string fileName = "PKGConfig.json";
	std::cout << "Creating a new PKGConfig.json in " << currentPath() << '\\' << fileName << endl;

	std::fstream file;
	string projectName;
	string creator;

	std::cout << "Please enter name for this project:" << endl;
	std::getline(std::cin, projectName);
	std::cout << "Please enter the creators name:" << std::endl;
	std::getline(std::cin, creator);

	// het beschrijven van het configuratie bestand.
	json jsonContent = json::parse(R"(
{
	"projectName": "val",
	"creator": "val",
	"version": "1.0.0",
	"dependencies": []
})");
	jsonContent["projectName"] = projectName;
	jsonContent["creator"] = creator;

	// De configuratie bestand aanmaken.
	file.open(currentPath() + '\\' + fileName, ios::out);
	file << jsonContent.dump(4) << endl;
	file.clear();
	fileName.clear();
	return true;
}
nlohmann::json ConfigHandler::GetConfigFile() {
	if (!ConfigHandler::ConfigExists())
	{
		std::cout << "No config found." << endl;
		exit(0);
	}
	string output = "";
	// variabelen in deze scope worden verwijderd zodra het compleet is. 
	{
		ifstream stream(currentPath() + '\\' + "PKGConfig.json");
		stringstream buffer;
		buffer << stream.rdbuf();
		stream.close();
		output = buffer.str();
		buffer.clear();
	}
	json config = json::parse(output);
	return config;
}

bool ConfigHandler::WriteToConfig(nlohmann::json& jsonToWrite)
{
	if (!ConfigHandler::ConfigExists())
	{
		cout << "PKGConfig.json file does not exist" << endl;
		exit(0);
	}
	cout << "Updating the PKGConfig file" << endl;
	fstream file;
	file.open(currentPath() + "\\PKGConfig.json", ios::out);
	file << jsonToWrite.dump(4);
	cout << "Written to file, closing and clearing stream" << endl;
	file.clear();
	file.close();
	cout << "Succesfully updated PKGConfig.json" << RESET << endl;
	return true;
}

bool ConfigHandler::ConfigExists()
{
	// check if the file PKGConfig exists.
	string dir = currentPath();
	string file = dir + "\\PKGConfig.json";
	ifstream stream;
	stream.open(file);
	if (stream)
	{
		stream.close();
		return true;
	}
	else {
		stream.close();
		return false;
	}
	return 0;
}
bool ConfigHandler::HasDependency(nlohmann::json& configFile, string& dependencyName) {
	vector<string> dependencies = configFile.get<vector<string>>();
	if (in_array(dependencyName, dependencies))
	{
		return true;
	}
	dependencies.clear();
	return false;
}