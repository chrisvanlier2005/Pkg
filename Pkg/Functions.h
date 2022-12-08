#pragma once
#include "Globals.h"
std::string currentPath();
bool in_array(const std::string &value, const std::vector<std::string> &array);
bool replace(std::string& str, const std::string& from, const std::string& to);