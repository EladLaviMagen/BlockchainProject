#pragma once
#include <map>
#include "Block.h"
class Blockchain
{

public:
	Blockchain();
	Blockchain(std::string chainInfo);
	int getCoinsOf(std::string user);
	void update(std::string newData);
	bool addBlock(Block* newBlock);
	void operator()();
	bool operator==(Blockchain& other);
	
private:
	std::map<std::string, Block*> chain;
	Block* cur_block;
};

