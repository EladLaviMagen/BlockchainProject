#pragma once
#include <iostream>
#include <string>
#include <time.h>
struct Header
{
	float version;
	int nonce;
	time_t timestamp;
	std::string targetHash;

}typedef Header;

class Block
{
public:
	Block(Header head, std::string prev);
	void addTransaction(std::string transaction);
	static std::string append(Block block);
	bool mine();






private:
	std::string _data;
	Header _header;
	std::string _prevHash;
};
