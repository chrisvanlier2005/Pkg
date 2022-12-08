#include "InstallationHandler.h"
#include "Globals.h"
#include <fstream>
using json = nlohmann::json;
using namespace std;
InstallationHandler::InstallationHandler(nlohmann::json& configuration)
	: configuration(configuration)
{
	this->configuration = configuration;
}

bool InstallationHandler::installPackage(std::string& language, std::string& packageName)
{
	std::string completeName = language + "/" + packageName;
	std::cout << "selected package: " << completeName << endl;
	if (this->hasPackage(completeName))
	{
		std::cout << RED << "You already have " + completeName + " installed" << RESET << std::endl;
		return false;
	}
	this->configuration["dependencies"].push_back(completeName);

	if (!this->DownloadPackage(completeName)) {
		// if something went wrong while downloading the package
		return false;
	};
	if (ConfigHandler::WriteToConfig(this->configuration)) {
		cout << GREEN << "Package installed succesfully." << RESET << endl;
		return true;
	}
	return false;
}

bool InstallationHandler::removePackage(std::string& language, std::string& packageName)
{
	string packagePath = currentPath() + "\\pkg_modules\\" + language + "-" + packageName + "." + language;
	cout << packagePath << endl;
	ifstream stream;
	stream.open(packagePath);
	if (!stream)
	{
		cout << "File does not exist" << endl;
		return false;
	}
	stream.close();
	try
	{
		std::filesystem::remove((currentPath() + "\\pkg_modules\\" + language + "-" + packageName + "." + language).c_str());
	}
	catch (const std::filesystem::filesystem_error& err) {
		std::cout << "filesystem error: " << err.what() << '\n';
	}

	try
	{
		string packName = language + '/' + packageName;
		cout << "Removing from Config" << endl;
		json config = ConfigHandler::GetConfigFile();
		vector<string> v = config["dependencies"].get<vector<string>>();
		auto itr = std::find(v.begin(), v.end(), packName);
		if (itr != v.end()) v.erase(itr);
		config["dependencies"] = v;
		ConfigHandler::WriteToConfig(config);
		cout << GREEN << "Package removed from config & filesystem" << RESET << endl;
		return true;
	}
	catch (const nlohmann::json::exception ex)
	{
		cout << RED << ex.what() << RESET << endl;
	}
	return false;
}

bool InstallationHandler::serverPackageList()
{
	return false;
}
bool InstallationHandler::DownloadPackage(std::string& packageName)
{
	// create the pkg_modules folder if it does not exist yet.
	std::filesystem::path path = currentPath() + "\\pkg_modules";
	if (!std::filesystem::is_directory(path))
	{
		cout << YELLOW << "pkg_modules does not exist  " << RESET << endl;
		cout << "Creating pkg_modules folder " << endl;
		std::filesystem::create_directory("pkg_modules");
		cout << GREEN << "Created successfully" << RESET << endl;
	}
	string url = "http://packagemanager.chrisvanlier.nl/install.php?package=" + packageName;
	string response = Request::Make(url);


	if (response == "404, package not found" || response == "404, package does not exist")
	{

		cout << RED << "Package does not exist" << RESET << endl;
		exit(0);
	}
	string::size_type index = packageName.find("/");
	string language = packageName.substr(0, index);
	replace(packageName, "/", "-");
	string outPath = currentPath() + "\\pkg_modules\\" + packageName + "." + language;


	std::ofstream file(outPath);

	file << response << endl;
	file.clear();
	file.close();
	return true;
}

bool InstallationHandler::hasPackage(string& packageName)
{  
	std::vector<std::string> dependencies = this->configuration["dependencies"].get<std::vector<std::string>>();
	if (in_array(packageName, dependencies))
	{
		return true;
	}
	
	return false;
}

