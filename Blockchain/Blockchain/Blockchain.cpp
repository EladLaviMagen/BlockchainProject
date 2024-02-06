#include "Blockchain.h"

Blockchain::Blockchain()
{
	
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

bool Blockchain::operator==(Blockchain& other)
{
	return true;
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