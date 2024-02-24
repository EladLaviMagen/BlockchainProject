#include "FileManager.h"


bool FileManager::saveRSA(std::string name, long long e, RSA cipher)
{
    std::ofstream file("C:/check/" + name + "RSA");
    if (!file.is_open())
    {
        // Print an error message if the file failed to open
        std::cerr << "Error: Unable to open RSA file " << std::endl;
        return false; // Return false to indicate failure
    }
    file << std::to_string(e) + "-" + std::to_string(cipher.getP()) + "-" + std::to_string(cipher.getQ());
    file.close();
    return true;
}

std::pair<long long, RSA> FileManager::loadRSA(std::string name)
{
    std::string str = "";
    std::string line = "";
    std::fstream file("C:/check/" + name + "RSA");
    if (!file.is_open())
    {
        // Print an error message if the file failed to open
        std::cerr << "Error: Unable to open RSA file " << std::endl;
        return std::pair<long long, RSA>(-1, RSA()); // Return false to indicate failure
    }
    char delim = EOF;
    while (std::getline(file, line, delim))
    {
        str += line;
    }
    std::vector<std::string> parts = splitString(str, '-');
    std::pair<long long, RSA> ret = std::pair<long long, RSA>(std::stoll(parts[0]), RSA(std::stoll(parts[1]), std::stoll(parts[2])));
    // Close the file
    file.close();
    return ret;
}

std::vector<std::string> FileManager::splitString(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(input);
    std::string token;

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

bool FileManager::save(std::string blockchain, std::string path)
{
    // Use the provided path argument to specify the output file path
    std::ofstream file(path);
    file.clear();
    // Check if the file was opened successfully
    if (!file.is_open())
    {
        // Print an error message if the file failed to open
        std::cerr << "Error: Unable to open file " << path << " for writing." << std::endl;
        return false; // Return false to indicate failure
    }

    // Write the blockchain data to the file
    file << blockchain;

    // Close the file
    file.close();

    return true; // Return true to indicate success
}

std::string FileManager::load(std::string path)
{
    std::string str = "";
    std::string line = "";
    // Use the provided path argument to specify the output file path
    std::fstream file(path);
    // Check if the file was opened successfully
    if (!file.is_open())
    {
        // Print an error message if the file failed to open
        std::cerr << "Error: Unable to open file " << path << " for writing." << std::endl;
        return "false"; // Return false to indicate failure
    }
    char delim = EOF;
    while (std::getline(file, line, delim))
    {
        str += line;
    }

    // Close the file
    file.close();

    return str; // Return true to indicate success
}

