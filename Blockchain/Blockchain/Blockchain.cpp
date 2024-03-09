#include "Blockchain.h"

Blockchain::Blockchain()
{
	Header head = { 1.0, 0, std::time(0), ""};
	cur_block = new Block(head, "0");
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

void Blockchain::printHistory()
{
	if (!chain.empty())
	{
		for (auto it = chain.begin(); it != chain.end(); it++)
		{
			std::cout << "-----------------Block------------------" << std::endl;
			it->second->printContents();
			std::cout << "-----------------MINED------------------" << std::endl;

		}
	}
}

void Blockchain::printCurTransactions()
{
	if (cur_block != nullptr)
	{
		std::cout << "-----------------Block------------------" << std::endl;
		cur_block->printContents();
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
	cur_block = nullptr;
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


float Blockchain::getCoinsOf(std::string user)
{
	float sum = 50;
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

void Blockchain::addTransaction(Transaction* tran)
{
	cur_block->addTransaction(tran);
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