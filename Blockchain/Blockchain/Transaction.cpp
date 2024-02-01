#include "Transaction.h"

int Transaction::t_id = 0;

Transaction::Transaction(int sum, std::string sender, std::string reciever, big* enc)
{
	RSA* cipher = new RSA();
	cipher->setP(enc[P]);
	cipher->setQ(enc[Q]);
	_sum = sum;
	_sender = sender;
	_reciever = reciever;
	_signature = cipher->rsaMain(getBaseSignature(), enc[PRIVATE]);
	_id = t_id;
	t_id++;
}

Transaction::Transaction(int sum, std::string sender, std::string reciever, std::vector<big> sig)
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

bool Transaction::verify(Transaction t, big dec)
{
	return false;
}
