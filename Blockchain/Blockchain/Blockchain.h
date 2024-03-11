#pragma once
#include <map>
#include "Block.h"

#define CHAIN_DELIM '\v'
class Blockchain
{

public:
	static bool mining;
	Blockchain();
	~Blockchain();
	void printHistory();
	void printCurTransactions();
	std::string toString();
	Blockchain(std::string chainInfo);
	float getCoinsOf(std::string user);
	void update(std::string newData);
	bool addTransaction(Transaction* tran);
	bool addBlock(Block* newBlock, std::string);
	std::string mine(std::string name);
	
private:
	std::map<std::string, Block*> chain;
	Block* cur_block;
};

