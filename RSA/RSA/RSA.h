#pragma once
#include <iostream>
#include <cmath>
#include <string>
#include <vector>

typedef long long big;

class RSA
{

public:
	//declarations
	RSA();
	big modInverse(big a);
	big modInverse(big a, big m);
	std::vector<big> rsaMain(std::vector<big> data, big k);
	big modExp(big base, big exp, big mod);
	bool setQ(big q);
	bool setP(big p);
	bool checkPublic(big e);

private:
	
	big generateRandomPrime();
	bool isPrime(big num);
	big _p;
	big _q;
	
};

