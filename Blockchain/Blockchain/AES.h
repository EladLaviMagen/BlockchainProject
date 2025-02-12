#pragma once
#include <iostream>
#include <cmath>
#include <cstdlib> 
#include <array>
#define SIZE 4
#define HEX 16
class AES
{
public:
	AES(std::string key);
	AES();
	~AES();
	std::string encrypt(std::string input);
	std::string decrypt(std::string input);
	void setKey(std::string newKey);
	std::string getKey();
	void GKey();
	static std::string trimString(std::string str);
private:

    static const unsigned char sbox[HEX][HEX];
	static const unsigned char inv_sbox[HEX][HEX];
	static const unsigned char CMDS[4][4];
	static const unsigned char INV_CMDS[4][4];
	unsigned char** _key;

	std::string matrixesToString(unsigned char*** input, int size);
	void encryptBlock(unsigned char** state, unsigned char** pkey);
	void decryptBlock(unsigned char** state, unsigned char** pkey);
	int calcBlocks(std::string input);
	unsigned char*** matrixAndPadString(std::string input);
	unsigned char*** allocateMatrixes(int size);
	unsigned char** allocateMatrix(int size);
	void copyKey(unsigned char** src, unsigned char** dst);
	void deallocateMatrixes(unsigned char*** msg, int size);
	void deallocateMatrix(unsigned char** msg, int size);
	void AddRoundKey(unsigned char** state, unsigned char** key);
	void ShiftRow(unsigned char** state, unsigned int i, unsigned int n);
	void ShiftRows(unsigned char** state);
	void SubBytes(unsigned char** state);
	void SubBytes(unsigned char* state);
	void InvShiftRows(unsigned char** state);
	void InvSubBytes(unsigned char** state);
	void InvMixColumns(unsigned char** state);
	void MixColumns(unsigned char** state);
	int getRcon(int round);
	void keyExpansion(unsigned char** key, int round);
	int aesMul(int a, int b);
};

