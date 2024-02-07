#include "Blockchain.h"

Blockchain::Blockchain()
{
	cur_block = nullptr;
}

Blockchain::~Blockchain()
{
	if (chain.size() != 0)
	{
		for (auto it = chain.begin(); it != chain.end(); it++)
		{
			delete it->second;
		}
	}
	if (cur_block != nullptr)
	{
		delete cur_block;
	}
	
}

std::string Blockchain::toString()
{
	std::string str = "";
	for (auto it = chain.begin(); it != chain.end(); it++)
	{
		str += (it->second)->append() + CHAIN_DELIM;
	}
	if (cur_block != nullptr)
	{
		str += cur_block->append();
	}
	return str;
}


Blockchain::Blockchain(std::string chainInfo)
{
	std::vector<std::string> blocks = FileManager::splitString(chainInfo, CHAIN_DELIM);
	if (blocks.size() < 2)
	{
		cur_block = new Block(blocks[0]);
	}
	else
	{
		for (int i = 0; i < blocks.size() - 1; i++)
		{
			Block* prev = new Block(blocks[i]);
			cur_block = new Block(blocks[i + 1]);
			chain[cur_block->getPrev()] = prev;
		}
	}
	
}


int Blockchain::getCoinsOf(std::string user)
{
	float sum = 0;
	if (chain.begin() != chain.end())
	{
		for (auto it = chain.begin(); it != chain.end(); it++)
		{
			sum += it->second->getCoins(user);
		}
	}
	return sum;
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