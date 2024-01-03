#include "Blockchain.h"

Blockchain::Blockchain()
{
}

Blockchain::Blockchain(std::string chainInfo)
{
}


int Blockchain::getCoinsOf(std::string user)
{
	return 0;
}

void Blockchain::update(std::string newData)
{
}

bool Blockchain::addBlock(Block* newBlock)
{
	return false;
}

void Blockchain::operator()()
{
	_block->mine();
}