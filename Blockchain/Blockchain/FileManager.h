#pragma once
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>

#define PATH "C:/check/save.txt"

class FileManager
{
public:
	static std::vector<std::string> splitString(const std::string& input, char delimiter);
	static bool save(std::string blockchain, std::string path);
	static std::string load(std::string path);
};

