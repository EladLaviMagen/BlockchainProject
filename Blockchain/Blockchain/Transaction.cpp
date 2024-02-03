#include "Transaction.h"

int Transaction::t_id = 0;

Transaction::Transaction(int sum, std::string sender, std::string reciever, big* enc)
{
	RSA cipher = RSA();
	cipher.setQandP(enc[Q], enc[P]);
	_sum = sum;
	_sender = sender;
	_reciever = reciever;
	_signature = cipher.rsaMain(getBaseSignature(), enc[KEY]);
	_id = t_id;
	t_id++;
}

Transaction::Transaction(int sum, std::string sender, std::string reciever, longString sig)
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

int Transaction::verify(Transaction t, big* dec)
{
	RSA cipher = RSA();
	if (!cipher.setQandP(dec[Q], dec[P]))
	{
		return BADNUMBERS;
	}
	longString signature = t._signature;
	longString base = t.getBaseSignature();
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
