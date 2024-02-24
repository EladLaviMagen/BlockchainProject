#pragma once
#include <sstream>
#include <vector>
#include <fstream>
#include <iostream>
#include <string>
#include "RSA.h"


#define PATH "C:/check/save.txt"

class FileManager
{
public:
	static bool saveRSA(std::string name, long long e, RSA cipher);
	static std::pair<long long, RSA> loadRSA(std::string name);
	static std::vector<std::string> splitString(const std::string& input, char delimiter);
	static bool save(std::string blockchain, std::string path);
	static std::string load(std::string path);
};

