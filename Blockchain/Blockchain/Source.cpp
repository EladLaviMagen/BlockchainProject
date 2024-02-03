#include <iostream>
#include "Block.h"
#pragma warning(disable: 4996)

#include <sstream>
#include <vector>
std::vector<std::string> splitString(const std::string& input, char delimiter);
#define TROLLAGE "0009320301a33902c9533651e4ab1f0799a7e9f4f53022d7a6728dbfeb91d83b"

int main()
{
    std::string str = "elad--";
    std::cin >> str;
    for (int i = 0; i < 10000; i++)
    {
        std::string hash = str + std::to_string(i);
        hash = SHA256::conv(hash);
        if (SHA256::comp(hash, TROLLAGE))
        {
            std::cout << "Y E S";
            break;
        }
        
    }
   
   /* RSA cipher = RSA();
    big e = cipher.generatePublic();
    big* nums = new big[3];
    nums[Q] = cipher.getQ();
    nums[P] = cipher.getP();
    nums[KEY] = e;
    
    Transaction t = Transaction(50, "Elad", "Maayan", nums);
    big d = cipher.modInverse(e);
    nums[KEY] = d;
    if (VERIFIED == Transaction::verify(t, nums))
    {
        std::cout << "We good!" << std::endl;
    }
    delete[] nums;*/
    return 0;
}

std::vector<std::string> splitString(const std::string& input, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(input);
    std::string token;

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}