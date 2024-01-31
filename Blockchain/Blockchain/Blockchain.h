#pragma once
#include <list>
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
	Block* _block;
	Block* _next;
};

