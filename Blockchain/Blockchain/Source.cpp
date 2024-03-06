#include <iostream>
#include "Blockchain.h"
#include "Peer.h"
#include <bitset>
#pragma warning(disable: 4996)



#define TROLLAGE "0009320301a33902c9533651e4ab1f0799a7e9f4f53022d7a6728dbfeb91d83b"


int main()
{
    Peer::start();
    return 0;
}

// Function to perform addition of two bitsets
std::bitset<512> add(const std::bitset<512>& a, const std::bitset<512>& b) {
    std::bitset<512> result;
    bool carry = false;
    for (int i = 0; i < 512; ++i) 
    {
        bool sum = a[i] ^ b[i] ^ carry;
        carry = (a[i] & b[i]) | (a[i] & carry) | (b[i] & carry);
        result[i] = sum;
    }
    return result;
}

// Function to perform multiplication of two bitsets
std::bitset<512> multiply(const std::bitset<512>& a, const std::bitset<512>& b) {
    std::bitset<512> result;
    for (int i = 0; i < 512; ++i)
    {
        if (b[i]) 
        {
            result ^= (a << i); // Shift a by i and then XOR with result
        }
    }
    return result;
}


