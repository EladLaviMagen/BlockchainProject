#include "Blockchain.h"

Blockchain::Blockchain()
{
	
}

std::string Blockchain::toString()
{
	std::string str = "";
	for (auto it = chain.begin(); it != chain.end(); it++)
	{
		str += Block::append(*(it->second)) + CHAIN_DELIM;
	}
	return str;
}

bool Blockchain::operator==(Blockchain& other)
{
	return true;
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

bool Blockchain::addBlock(Block* newBlock, std::string prev)
{
	chain[prev] = newBlock;
	return true;
}

void Blockchain::operator()()
{
	cur_block->mine();
}