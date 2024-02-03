#pragma once
#include <iostream>
#include <string>
#include "RSA.h"


#define DELIMETER '\n'
#define SIG_DELIMETER '-'
#define BADNUMBERS 0
#define VERIFIED 1
#define FAILED 2

#define KEY 0
#define P 1
#define Q 2
class Transaction
{

public:
	Transaction(int sum, std::string sender, std::string reciever, big* enc);
	Transaction(int sum, std::string sender, std::string reciever, longString sig);
	static int verify(Transaction t, big* dec);
	std::string toString();
	
private:
	longString getBaseSignature();
	static int t_id;
	int _id;
	int _sum;
	std::string _sender;
	std::string _reciever;
	longString _signature;
};

