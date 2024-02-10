#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

typedef long long big;
typedef std::vector<big> longString;

class RSA
{

public:
	//declarations
	RSA();
	big modInverse(big a);
	big modInverse(big a, big m);
	longString rsaMain(longString data, big k);
	big modExp(big base, big exp, big mod);
	bool setQandP(big q, big p);
	bool checkPublic(big e);
	big generatePublic();
	big getP();
	big getQ();

private:
	
	big generateRandomPrime();
	bool isPrime(big num);
	big _p;
	big _q;
	
};

