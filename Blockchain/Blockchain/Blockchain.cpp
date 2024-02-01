#include "Blockchain.h"

Blockchain::Blockchain()
{
	
}

bool Blockchain::operator==(Blockchain& other)
{
	if (_block == other._block && _next == other._next)
	{
		return true;
	}
	do
	{
		if (!(*_block == *other._block))
		{
			return false;
		}
	} while (_next != nullptr);
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
	_block->mine();
}