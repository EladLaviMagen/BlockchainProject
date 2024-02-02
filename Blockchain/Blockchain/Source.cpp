#include <iostream>
#include "Transaction.h"
#pragma warning(disable: 4996)


int main()
{
    RSA cipher = RSA();
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
        std::cout << "We good!";
    }
    delete[] nums;
    return 0;
}