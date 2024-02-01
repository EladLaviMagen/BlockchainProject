#pragma once
#include <iostream>
#include <string>
#include "RSA.h"

#define PRIVATE 0
#define P 1
#define Q 2
class Transaction
{

public:
	Transaction(int sum, std::string sender, std::string reciever, big* enc);
	Transaction(int sum, std::string sender, std::string reciever, std::vector<big> sig);
	static bool verify(Transaction t, big dec);
	std::vector<big> getSignedSignature();

private:
	std::vector<big> getBaseSignature();
	static int t_id;
	int _id;
	int _sum;
	std::string _sender;
	std::string _reciever;
	std::vector<big> _signature;
};

