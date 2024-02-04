#include <iostream>
#include "Blockchain.h"
#pragma warning(disable: 4996)
#include <sstream>
std::vector<std::string> splitString(const std::string& input, char delimiter);
#define TROLLAGE "0009320301a33902c9533651e4ab1f0799a7e9f4f53022d7a6728dbfeb91d83b"


int main()
{
    time_t time = 55;
    Header head = { 1, 2, time, "Elad" };
    std::string str = "";
    Blockchain b = Blockchain();
    Block* genesis = new Block(head, "0");
    Block* first = new Block(head, "1");
    Block* second = new Block(head, "2");


    RSA cipher = RSA();
    big e = cipher.generatePublic();
    big* nums = new big[3];
    nums[Q] = cipher.getQ();
    nums[P] = cipher.getP();
    nums[KEY] = e;

    Transaction* t = new Transaction(50, "Elad", "Maayan", nums);
    Transaction* t1 = new Transaction(25, "Elad", "Maayan", nums);
    Transaction* t2 = new Transaction(10, "Elad", "Maayan", nums);
    first->addTransaction(t);
    first->addTransaction(t1);
    first->addTransaction(t2);

    second->addTransaction(t2);
    second->addTransaction(t1);
    second->addTransaction(t);
    b.addBlock(genesis, "0");
    b.addBlock(first, "1");
    b.addBlock(second, "2");
    str = b.toString();

    delete genesis;
    delete first;
    delete second;
   
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