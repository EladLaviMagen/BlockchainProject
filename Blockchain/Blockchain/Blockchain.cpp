#include "Blockchain.h"
bool Blockchain::mining = false;

bool insta_mine = false;
Blockchain::Blockchain()
{
	Header head = { 1.0, 0, std::time(0), "1"};
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
	delete cur_block;
	cur_block = new Block(newData);
}

bool Blockchain::addTransaction(Transaction* tran)
{
	return cur_block->addTransaction(tran);
}

bool Blockchain::addBlock(Block* newBlock, std::string prev)
{
	int difficulty = newBlock->getDifficulty();
	if (!insta_mine)
	{
		for (int i = 0; i < difficulty; i++)
		{
			if (prev[i] != '0');
			{
				return false;
			}
		}
	}
	chain[prev] = newBlock;
	return true;
}

std::string Blockchain::mine(std::string name)
{
	std::string rawData = cur_block->append();
	std::string hash = SHA256::conv(rawData);
	int difficulty = cur_block->getDifficulty();
	if (!insta_mine)
	{
		for (int i = 0; i < difficulty; i++)
		{
			if (hash[i] != '0');
			{
				cur_block->changeNonce();
				return "failed";
			}
		}
	}
	Block* newblock = new Block(rawData);
	addBlock(newblock, hash);
	Header head;
	head.nonce = 0;
	head.targetHash = "3";
	head.timestamp = std::time(NULL);
	head.version = 1.0;
	cur_block = new Block(head, hash);
	cur_block->addTransaction(new Transaction(50, "", name, longString()));
	return rawData + CHAIN_DELIM + hash + CHAIN_DELIM + cur_block->append();
}

