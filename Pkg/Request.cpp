#include "Request.h"
#include "Globals.h"
using namespace std;
string Request::Make(std::string url)
{
	Request::init();
	
	// initialize curlpp and make a request to the given url
	curlpp::Easy easyhandle;
	std::ostringstream os;
	curlpp::options::WriteStream ws(&os);
	easyhandle.setOpt<curlpp::options::Url>(url);
	easyhandle.setOpt(ws);
	easyhandle.perform();
	return string(os.str());
}

bool Request::DownloadZip(std::string url)
{
	return false;
}

bool Request::init() {
	return true;
}

