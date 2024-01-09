#include "SHA256.h"
#include <iostream>
int main()
{
	SHA256* hash = new SHA256();
	std::cout << hash->conv("a");
	delete hash;
}