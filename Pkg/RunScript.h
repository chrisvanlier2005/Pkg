#pragma once
#include "Globals.h";

class RunScript
{
public:
	static bool validate(const std::string& script_name);
	static bool run(const std::string& script_name);
};
