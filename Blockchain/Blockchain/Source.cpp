#include <iostream>
#include "Blockchain.h"
#pragma warning(disable: 4996)


#define TROLLAGE "0009320301a33902c9533651e4ab1f0799a7e9f4f53022d7a6728dbfeb91d83b"


int main()
{

    time_t time = std::time(nullptr);
    Header head = { 1, 2, time, "Rah" };
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

    Transaction* t12 = new Transaction(50, "Elad", "Maayan", nums);
    Transaction* t11 = new Transaction(25, "Elad", "Maayan", nums);
    Transaction* t21 = new Transaction(10, "Elad", "Maayan", nums);
    second->addTransaction(t21);
    second->addTransaction(t11);
    second->addTransaction(t12);
    b.addBlock(genesis, "0");
    b.addBlock(first, "1");
    b.addBlock(second, "2");
    str = b.toString();
    Blockchain check = Blockchain(str);
    std::string checkstr = check.toString();
    FileManager::save(checkstr, PATH);
    std::string loaded = FileManager::load(PATH);
    std::cout << b.getCoinsOf("Elad") << std::endl;
    if (loaded == checkstr)
    {
        std::cout << "Legendery!";
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

