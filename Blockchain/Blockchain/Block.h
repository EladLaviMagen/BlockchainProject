#pragma once
#include <iostream>
#include <string>
#include <time.h>
#include "Transaction.h"

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
	void addTransaction(Transaction transaction);
	static std::string append(Block block);
	bool mine();
	bool operator==(Block& other);

private:
	std::vector<Transaction> _data;
	Header _header;
	std::string _prevHash;
};

