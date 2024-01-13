#include "Block.h"

Block::Block(Header head, std::string prev)
{
	_header = head;
	_data = "";
	_prevHash = prev;
	
}

void Block::addTransaction(std::string transaction)
{
	_data += transaction;
}

std::string Block::append(Block block)
{
	std::string head = "" + block._header.nonce + block._header.targetHash + block._header.timestamp + std::to_string(block._header.version) + block._prevHash;
	//For now, placeholder for what is to come
	/*
	* Insert function which appends transactions into the string
	*/
	return head + block._data;
}


bool Block::mine()
{
	std::string rawData = append(*this);
	/*
	* Insert hashing function and checking with target hash
	*/
	return false;
}

