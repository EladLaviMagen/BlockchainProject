#pragma once
#include <sstream>
#include <vector>
#include <iostream>
#include <string>
class FileManager
{
public:
	static std::vector<std::string> splitString(const std::string& input, char delimiter);
};

