#include "Block.h"

Block::Block(Header head, std::string prev)
{
	_header = head;
	_prevHash = prev;
	
}

bool Block::addTransaction(Transaction* transaction)
{
	if (_data.size() < MAX)
	{
		_data.push_back(transaction);
		return true;
	}
	return false;
	
}

bool Block::operator==(Block& other)
{
	if (this->_header.version != other._header.version)
	{
		return false;
	}
	if (this->_header.timestamp != other._header.timestamp)
	{
		return false;
	}
	if (this->_header.version != other._header.version)
	{
		return false;
	}
	if (this->_prevHash != other._prevHash)
	{
		return false;
	}
	if (this->_data != other._data)
	{
		return false;
	}
	return true;
}

std::string Block::append(Block block)
{
	std::string block_str = std::to_string(block._header.nonce) + BLOCK_DELIMETER + block._header.targetHash + BLOCK_DELIMETER + std::to_string(block._header.timestamp) + BLOCK_DELIMETER + std::to_string(block._header.version) + BLOCK_DELIMETER + block._prevHash + BLOCK_DELIMETER;
	for (int i = 0; i < block._data.size(); i++)
	{
		block_str += block._data[i]->toString();
		if (i != block._data.size() - 1)
		{
			block_str += TRANSACTION_DELI;
		}
	}
	return block_str;
}


bool Block::mine()
{
	std::string rawData = append(*this);
	std::string hash = SHA256::conv(rawData);
	//if()
	return false;
}

