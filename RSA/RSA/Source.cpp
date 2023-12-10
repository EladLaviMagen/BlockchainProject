#include <iostream>
#include <cmath>


long long modInverse(long long a, long long m);
long long rsaDecrypt(long long ciphertext, long long d, long long p, long long q);


int main() 
{
    long long x = modInverse(17, 3);
    unsigned long long p = 7919; // Replace with your prime p
    unsigned long long q = 5839; // Replace with your prime q
    long long N = p * q;
    long long phi = (p - 1) * (q - 1);

    long long e = 17; // Replace with your public exponent e
    long long d = modInverse(e, phi); // Calculate private exponent d

    long long plaintext = 42; // Replace with your plaintext
    std::cout << "Original plaintext: " << plaintext << std::endl;

    // Encryption
    long long ciphertext = modExp(plaintext, e, N);
    std::cout << "Encrypted ciphertext: " << ciphertext << std::endl;

    // Decryption using CRT
    long long decryptedText = rsaDecrypt(ciphertext, d, p, q);
    std::cout << "Decrypted plaintext using CRT: " << decryptedText << std::endl;

    return 0;
}

// Function to perform modular exponentiation (base^exp % mod)
long long modExp(long long base, long long exp, long long mod) {
    long long result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
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