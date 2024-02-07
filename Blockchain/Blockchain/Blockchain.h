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
	int getCoinsOf(std::string user);
	void update(std::string newData);
	bool addBlock(Block* newBlock, std::string);
	void operator()();
	bool operator==(Blockchain& other);
	
private:
	std::map<std::string, Block*> chain;
	Block* cur_block;
};

