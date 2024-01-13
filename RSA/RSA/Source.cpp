#include <iostream>
#include <cmath>

//declarations
long long modInverse(long long a, long long m);
long long rsaDecrypt(long long ciphertext, long long d, long long p, long long q);
long long modExp(long long base, long long exp, long long mod);

int main() 
{
    unsigned long long p = 97579; // Replace with your prime p
    unsigned long long q = 97583; // Replace with your prime q
    long long N = p * q;
    long long totient = (p - 1) * (q - 1);

    long long e = 999983; // Replace with your public exponent e
    long long d = modInverse(e, totient); // Calculate private exponent d
    for (int i = 0; i < 300; i++)
    {
        long long plaintext = std::rand() % 122 + 61;  // Replace with your plaintext
        std::cout << "Original plaintext: " << plaintext << std::endl;

        //Encryption
        long long ciphertext = rsaDecrypt(plaintext, e, p, q);
        std::cout << "Encrypted ciphertext: " << ciphertext << std::endl;

        // Decryption using CRT
        long long decryptedText = rsaDecrypt(ciphertext, d, p, q);
        std::cout << "Decrypted plaintext using CRT: " << decryptedText << std::endl;
    }
    

    return 0;
}

/* 
* Function to perform modular exponentiation(base^exp % mod)
* Input : base, exponent and number to modulo
* Output : result
*/
long long modExp(long long base, long long exp, long long mod) 
{
    //Setting up
    long long result = 1;
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
long long modInverse(long long a, long long m) {
    long long m0 = m;
    long long y = 0, x = 1;
    if (m == 1) {
        return 0;
    }
    while (a > 1) {
        long long q = a / m;
        long long t = m;
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



// RSA decryption using CRT
long long rsaDecrypt(long long ciphertext, long long d, long long p, long long q) {
    long long dp = d % (p - 1);
    long long dq = d % (q - 1);

    long long mp = modExp(ciphertext, dp, p);
    long long mq = modExp(ciphertext, dq, q);

    long long qInv = modInverse(q, p);
    long long pInv = modInverse(p, q);

    return (mp * q * qInv + mq * p * pInv) % (p * q);
}