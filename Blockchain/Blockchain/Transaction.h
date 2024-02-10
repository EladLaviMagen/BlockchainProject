#pragma once
#include "RSA.h"
#include "FileManager.h"

#define DELIMETER '\n'
#define SIG_DELIMETER '-'
#define BADNUMBERS 0
#define VERIFIED 1
#define FAILED 2


#define ID 0
#define RECV 1
#define SENDER 2
#define SUM 3

#define KEY 0
#define P 1
#define Q 2


class Transaction
{

public:
	Transaction(float sum, std::string sender, std::string reciever, big* enc);
	Transaction(float sum, std::string sender, std::string reciever, longString sig);
	Transaction(std::string str);
	static int verify(Transaction t, big* dec);
	std::string getRecv();
	std::string getSender();
	float getSum();
	std::string toString();
	
private:
	longString getBaseSignature();
	static int t_id;
	int _id;
	float _sum;
	std::string _sender;
	std::string _reciever;
	longString _signature;
};

