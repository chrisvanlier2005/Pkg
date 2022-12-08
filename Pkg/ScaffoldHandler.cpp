#include "ScaffoldHandler.h"
#include <stdio.h>
#include <fstream>
#include <miniz/miniz.h>
using namespace std;
bool ScaffoldHandler::Install(std::string& scaffoldName)
{
	{
		string validateUrl = "http://packagemanager.chrisvanlier.nl/scaffold.php?exist=" + scaffoldName;
		string existsOut = Request::Make(validateUrl);
		if (existsOut != "200, scaffold exists")
		{
			cout << RED << "Scaffold: " << scaffoldName << " Does not exist." << RESET << endl;
			exit(0);
		}
	}
	string outfile = currentPath() + "\\scaffold.zip";
	string url = "http://packagemanager.chrisvanlier.nl/scaffold.php?find=" + scaffoldName;
	try {
		cout << "Downloading scaffold" << endl;
		FILE* fp;
		curlpp::Easy easy;

		
		curlpp::options::WriteFunctionCurlFunction
			myFunction(ScaffoldHandler::WriteCallback);
		fp = fopen(outfile.c_str(), "wb");
		curlpp::OptionTrait<void*, CURLOPT_WRITEDATA>
			myData(fp);

		easy.setOpt(myFunction);
		easy.setOpt(myData);
		easy.setOpt(new curlpp::options::Url(url));
		easy.perform();
		cout << "Download succesfull, closing stream...";
		fclose(fp);
		cout << BLUE << "Closed stream" << RESET << endl;
		cout << "Installing on local device" << endl;
		ScaffoldHandler::ExtractAndRemove(outfile);
	}
	catch (curlpp::RuntimeError& e) {
		std::cerr << "curlpp::RuntimeError: " << e.what() << std::endl;
	}
	catch (curlpp::LogicError& e) {
		std::cerr << "curlpp::LogicError: " << e.what() << std::endl;
	}
	return true;
}


size_t ScaffoldHandler::WriteCallback(char* ptr, size_t size, size_t nmemb, void* f)
{
	FILE* file = (FILE*)f;
	return fwrite(ptr, size, nmemb, file);
};

// extract the zip and remove it from disk.
bool ScaffoldHandler::ExtractAndRemove(string& zipPath) {
	std::string zipfile = zipPath;
	mz_zip_archive zip_archive;
	memset(&zip_archive, 0, sizeof(zip_archive));
	mz_bool status = mz_zip_reader_init_file(&zip_archive, zipfile.c_str(), 0);
	if (!status) {
		std::cerr << "Failed to open " << zipfile << std::endl;
		return 1;
	}
	// Get the number of files in the zip archive
	uint32_t file_count = mz_zip_reader_get_num_files(&zip_archive);

	// Extract each file in the zip archive
	for (uint32_t i = 0; i < file_count; i++) {
		// Get information about the file
		mz_zip_archive_file_stat file_stat;
		if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
			std::cerr << "Failed to get information about file " << i << " in " << zipfile << std::endl;
			continue;
		}

		// Open the file for reading
		void* file_data = mz_zip_reader_extract_to_heap(&zip_archive, file_stat.m_file_index, &file_stat.m_uncomp_size, 0);
		if (!file_data) {
			std::cerr << "Failed to extract " << file_stat.m_filename << " from " << zipfile << std::endl;
			continue;
		}

		// Create a local file to write the extracted data to
		std::string outfile = file_stat.m_filename;

		string directory;
		const size_t last_slash_idx = outfile.rfind('/');
		if (std::string::npos != last_slash_idx)
		{
			directory = outfile.substr(0, last_slash_idx);
			if (!std::filesystem::exists(directory))
			{
				std::filesystem::create_directories(directory);
			}
		}
		std::ofstream ofs(outfile.c_str(), std::ios::binary);
		if (ofs) {
			// Write the extracted data to the local file
			ofs.write((const char*)file_data, file_stat.m_uncomp_size);
			ofs.close();

			// Print a message indicating that the file was extracted successfully
			std::cout << "Extracted " << outfile << " from " << zipfile << std::endl;
		}
		else {
			std::cerr << "Failed to open " << outfile << " for writing" << std::endl;
		}

		// Clean up
		free(file_data);
	}
	// Close the zip file
	mz_zip_reader_end(&zip_archive);
	std::remove(zipPath.c_str());
}
