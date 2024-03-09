#pragma once
#include <map>
#include "Block.h"

#define CHAIN_DELIM '\v'
class Blockchain
{

public:
	Blockchain();
	~Blockchain();
	void printHistory();
	void printCurTransactions();
	std::string toString();
	Blockchain(std::string chainInfo);
	float getCoinsOf(std::string user);
	void update(std::string newData);
	void addTransaction(Transaction* tran);
	bool addBlock(Block* newBlock, std::string);
	void operator()();
	
private:
	std::map<std::string, Block*> chain;
	Block* cur_block;
};

