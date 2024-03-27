#include "RSA.h"


// Modular exponentiation function (a^b mod n)
big RSA::modPow(big base, big exponent, big modulus) {
    big result = 1;
    base = base % modulus;
    while (exponent > 0) {
        if (exponent & 1) {
            result = (result * base) % modulus;
        }
        base = (base * base) % modulus;
        exponent >>= 1;
    }
    return result;
}

// Miller-Rabin primality test
bool RSA::isPrimeMiller(big n) {
    if (n <= 1 || n == 4) return false;
    if (n <= 3) return true;

    // Find d such that d * 2^r = n - 1
    big d = n - 1;
    while (d % 2 == 0) {
        d /= 2;
    }

    // Perform Miller-Rabin test k times
    for (int i = 0; i < 10; ++i) {
        big a = 2 + rand() % (n - 3);
        big x = modPow(a, d, n);
        if (x == 1 || x == n - 1) continue;

        bool isComposite = true;
        for (big r = 1; r < d; r *= 2) {
            x = modPow(a, r * d, n);
            if (x == n - 1) {
                isComposite = false;
                break;
            }
        }
        if (isComposite) return false;
    }
    return true;
}
RSA::RSA()
{
    _p = generateRandomPrime(); // Replace with your prime p
    _q = generateRandomPrime(); // Replace with your prime q
    while (_q == _p)
    {
        _q = generateRandomPrime();
    }
}
RSA::RSA(big p, big q)
{
    if (q == p || !isPrime(p) || !isPrime(q))
    {
        throw(std::exception());
    }
    _q = q;
    _p = p;
    
}
big RSA::generatePublic()
{
    srand(time(0)); // Seed for random number generation
    big randomNum = 0;
    do 
    {
        randomNum = rand() % 1000000;
    } while (!checkPublic(randomNum));
    return randomNum;
}
big RSA::getP()
{
    return _p;
}

big RSA::getQ()
{
    return _q;
}
/*
* Function to perform modular exponentiation(base^exp % mod)
* Input : base, exponent and number to modulo
* Output : result
*/
big RSA::modExp(big base, big exp, big mod)
{
    //Setting up
    big result = 1;
    base = base % mod;
    //This function uses binary exponention
    while (exp > 0)
    {
        //We use the binary value of the number to then include the exponention
        if (exp % 2 == 1)
        {
            //Multiplying result
            result = (result * base) % mod;
        }
        //base^2, base^4, base^8...
        base = (base * base) % mod;
        //dividing exponent
        exp /= 2;
    }
    return result;
}

// Function to calculate modular multiplicative inverse using Extended Euclidean Algorithm
big RSA::modInverse(big a)
{
    big m = (_p - 1) * (_q - 1);
    big m0 = m;
    big y = 0, x = 1;
    if (m == 1) {
        return 0;
    }
    while (a > 1) {
        big q = a / m;
        big t = m;
        m = a % m, a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < 0) {
        x += m0;
    }
    return x;
}

big RSA::modInverse(big a, big m)
{
    big m0 = m;
    big y = 0, x = 1;
    if (m == 1) {
        return 0;
    }
    while (a > 1) {
        big q = a / m;
        big t = m;
        m = a % m, a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < 0) {
        x += m0;
    }
    return x;
}

bool RSA::checkPublic(big e)
{
    big t = (_p - 1) * (_q - 1);
    if (isPrime(e))
    {
        if (e < t)
        {
            if (t % e != 0)
            {
                return true;
            }
        }
    }
    return false;
}


// RSA decryption using CRT
longString RSA::rsaMain(longString data, big k)
{
    big dp = k % (_p - 1);
    big dq = k % (_q - 1);
    for (size_t i = 0; i < data.size(); i++)
    {
        big mp = modExp(data[i], dp, _p);
        big mq = modExp(data[i], dq, _q);

        big qInv = modInverse(_q, _p);
        big pInv = modInverse(_p, _q);

        data[i] = (mp * _q * qInv + mq * _p * pInv) % (_p * _q);
    }
    return data;
}


bool RSA::isPrime(big num) {
    
    for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

big RSA::generateRandomPrime() 
{
    srand(time(0)); // Seed for random number generation

    big lowerBound = 100000;
    big upperBound = 200000;

    big randomNum = 0;
    bool flag = false;
    do {
        randomNum = (rand() % (upperBound - lowerBound + 1)) + lowerBound;
        randomNum = randomNum << 1;
        randomNum++;
        if (isPrimeMiller(randomNum))
        {
            if (isPrime(randomNum))
            {
                flag = true;
            }
        }
    } while (!flag);

    return randomNum;
}

bool RSA::setQandP(big q, big p)
{
    if (isPrime(q) && isPrime(p) && p != q)
    {
        _q = q;
        _p = p;
        return true;
    }
    return false;
}
