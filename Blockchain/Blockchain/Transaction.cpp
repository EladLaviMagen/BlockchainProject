#include "Transaction.h"

int Transaction::t_id = 0;

Transaction::Transaction(float sum, std::string sender, std::string reciever, big* enc)
{
	RSA cipher = RSA(enc[RSA_P], enc[RSA_Q]);
	_sum = sum;
	_sender = sender;
	_reciever = reciever;
	_id = t_id;
	_signature = cipher.rsaMain(getBaseSignature(), enc[KEY]);
	t_id++;
}

Transaction::Transaction(float sum, std::string sender, std::string reciever, longString sig)
{
	_sum = sum;
	_sender = sender;
	_reciever = reciever;
	_id = t_id;
	t_id++;
	for (int i = 0; i < sig.size(); i++)
	{
		_signature.push_back(sig[i]);
	}
}



int Transaction::verify(big* dec)
{
	RSA cipher = RSA();
	if (!cipher.setQandP(dec[RSA_Q], dec[RSA_P]))
	{
		return BADNUMBERS;
	}
	longString signature = this->_signature;
	longString base = this->getBaseSignature();
	signature = cipher.rsaMain(signature, dec[KEY]);
	if (signature.size() != base.size())
	{
		return FAILED;
		
	}
	for (int i = 0; i < signature.size(); i++)
	{
		if (signature[i] != base[i])
		{
			return FAILED;
		}
	}
	return VERIFIED;
}

std::string Transaction::getRecv()
{
	return _reciever;
}

std::string Transaction::getSender()
{
	return _sender;
}

float Transaction::getSum()
{
	return _sum;
}

Transaction::Transaction(std::string str)
{
	std::vector<std::string> info = FileManager::splitString(str, DELIMETER);
	
	_id = std::stoi(info[ID]);
	_sum = std::stof(info[SUM]);
	_sender = info[SENDER];
	_reciever = info[RECV];
	if (info.size() == 5)
	{
		std::vector<std::string> signature = FileManager::splitString(info[info.size() - 1], SIG_DELIMETER);
		for (int i = 0; i < signature.size(); i++)
		{
			_signature.push_back(std::stoll(signature[i]));
		}
	}
	t_id++;

}

std::string Transaction::toString()
{
	std::string data = std::to_string(this->_id) + DELIMETER + this->_reciever + DELIMETER + this->_sender + DELIMETER + std::to_string(this->_sum) + DELIMETER;
	std::string signature_str = "";
	for (int i = 0; i < _signature.size(); i++)
	{
		signature_str += std::to_string(_signature[i]);
		if (i != _signature.size() - 1)
		{
			signature_str += SIG_DELIMETER;
		}
	}
	return data + signature_str;
}


longString Transaction::getBaseSignature()
{
	longString signature;
	std::string baseSig = std::to_string(this->_id) + this->_reciever + this->_sender + std::to_string(this->_sum);
	for (int i = 0; i < baseSig.size(); i++)
	{
		signature.push_back(baseSig[i]);
	}
	return signature;
}
