#include "Blockchain.h"

Blockchain::Blockchain()
{
	
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

bool Blockchain::addBlock(Block* newBlock)
{
	return false;
}

void Blockchain::operator()()
{
	cur_block->mine();
}