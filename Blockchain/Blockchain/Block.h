#pragma once
#include <time.h>
#include "Transaction.h"
#include "SHA256.h"

#define BLOCK_DELIMETER '\t'
#define TRANSACTION_DELI '\r'
#define MAX 20

#define NONCE 0
#define TARGET 1
#define TIMESTAMP 2
#define VERSION 3
#define PREV 4

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
	Block(std::string str);
	void changeNonce();
	void printContents();
	float getCoins(std::string user);
	~Block();
	bool addTransaction(Transaction* transaction);
	std::string append();
	bool mine();
	bool operator==(Block& other);
	std::string getPrev();

private:
	std::vector<Transaction*> _data;
	Header _header;
	std::string _prevHash;
};

