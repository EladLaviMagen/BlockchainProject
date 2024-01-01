#pragma once
#include <iostream>
#include <time.h>
struct Header
{
	float version;
	int nonce;
	time_t timestamp;
	int targetHash;

}typedef Header;
class Block
{
public:
	Block(Header head, std::string miner, int prev);
	static std::string append(Block block);
	bool mine();
	void operator()();





private:
	std::string _data;
	Header _header;
	int _prevHash;
};

