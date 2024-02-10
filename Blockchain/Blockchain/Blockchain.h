#pragma once
#include <map>
#include "Block.h"

#define CHAIN_DELIM '\v'
class Blockchain
{

public:
	Blockchain();
	~Blockchain();
	std::string toString();
	Blockchain(std::string chainInfo);
	float getCoinsOf(std::string user);
	void update(std::string newData);
	bool addBlock(Block* newBlock, std::string);
	void operator()();
	
private:
	std::map<std::string, Block*> chain;
	Block* cur_block;
};

