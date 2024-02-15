#include "AES.h"

const unsigned char AES::sbox[HEX][HEX] = {
   {0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b,
    0xfe, 0xd7, 0xab, 0x76},
   {0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf,
    0x9c, 0xa4, 0x72, 0xc0},
   {0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1,
    0x71, 0xd8, 0x31, 0x15},
   {0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2,
    0xeb, 0x27, 0xb2, 0x75},
   {0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3,
    0x29, 0xe3, 0x2f, 0x84},
   {0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39,
    0x4a, 0x4c, 0x58, 0xcf},
   {0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f,
    0x50, 0x3c, 0x9f, 0xa8},
   {0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21,
    0x10, 0xff, 0xf3, 0xd2},
   {0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d,
    0x64, 0x5d, 0x19, 0x73},
   {0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14,
    0xde, 0x5e, 0x0b, 0xdb},
   {0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62,
    0x91, 0x95, 0xe4, 0x79},
   {0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea,
    0x65, 0x7a, 0xae, 0x08},
   {0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f,
    0x4b, 0xbd, 0x8b, 0x8a},
   {0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9,
    0x86, 0xc1, 0x1d, 0x9e},
   {0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9,
    0xce, 0x55, 0x28, 0xdf},
   {0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f,
    0xb0, 0x54, 0xbb, 0x16} };
const unsigned char AES::inv_sbox[HEX][HEX] = {
    {0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e,
     0x81, 0xf3, 0xd7, 0xfb},
    {0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44,
     0xc4, 0xde, 0xe9, 0xcb},
    {0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b,
     0x42, 0xfa, 0xc3, 0x4e},
    {0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49,
     0x6d, 0x8b, 0xd1, 0x25},
    {0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc,
     0x5d, 0x65, 0xb6, 0x92},
    {0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57,
     0xa7, 0x8d, 0x9d, 0x84},
    {0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05,
     0xb8, 0xb3, 0x45, 0x06},
    {0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03,
     0x01, 0x13, 0x8a, 0x6b},
    {0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce,
     0xf0, 0xb4, 0xe6, 0x73},
    {0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8,
     0x1c, 0x75, 0xdf, 0x6e},
    {0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e,
     0xaa, 0x18, 0xbe, 0x1b},
    {0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe,
     0x78, 0xcd, 0x5a, 0xf4},
    {0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59,
     0x27, 0x80, 0xec, 0x5f},
    {0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f,
     0x93, 0xc9, 0x9c, 0xef},
    {0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c,
     0x83, 0x53, 0x99, 0x61},
    {0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63,
     0x55, 0x21, 0x0c, 0x7d} };
const unsigned char AES::CMDS[4][4] = {
    {2, 3, 1, 1}, {1, 2, 3, 1}, {1, 1, 2, 3}, {3, 1, 1, 2} };
const unsigned char AES::INV_CMDS[4][4] = {
    {14, 11, 13, 9}, {9, 14, 11, 13}, {13, 9, 14, 11}, {11, 13, 9, 14} };

AES::AES(std::string key)
{
    int size = calcBlocks(key);
    unsigned char*** prepKey = matrixAndPadString(key);
    _key = allocateMatrix(SIZE);
    copyKey(prepKey[0], _key);
    deallocateMatrixes(prepKey, size);
}

AES::AES()
{
    _key = allocateMatrix(SIZE);
    GKey();
}

void AES::setKey(std::string newKey)
{
    int size = calcBlocks(newKey);
    unsigned char*** prepKey = matrixAndPadString(newKey);
    copyKey(prepKey[0], _key);
    deallocateMatrixes(prepKey, size);
}

AES::~AES()
{
    deallocateMatrix(_key, SIZE);
}

std::string AES::getKey()
{
    std::string key = matrixesToString(&_key, 1);
    return key;
}

std::string AES::encrypt(std::string input)
{
    unsigned char*** states = matrixAndPadString(input);
    int size = calcBlocks(input);
    for (int i = 0; i < size; i++)
    {
        encryptBlock(states[i], _key);
    }
    std::string result = matrixesToString(states, size);
    return result;
}

std::string AES::decrypt(std::string input)
{
    unsigned char*** states = matrixAndPadString(input);
    int size = calcBlocks(input);
    for (int i = 0; i < size; i++)
    {
        decryptBlock(states[i], _key);
    }
    std::string result = matrixesToString(states, size);
    return result;
}
int AES::calcBlocks(std::string input)
{
    int size = input.length() / 16;
    if (input.length() % 16 != 0)
    {
        size++;
    }
    return size;
}

std::string AES::matrixesToString(unsigned char*** input, int size)
{
    std::string res = "";
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            for (int k = 0; k < SIZE; k++)
            {
               
               //std::cout << std::hex << (int)input[i][k][j] << " ";
                res += input[i][k][j];
            }
        }
    }
    //std::cout << std::endl;
    return res;
} 

unsigned char*** AES::matrixAndPadString(std::string input)
{
    int i = 0;
    int size = calcBlocks(input);
    unsigned char*** matrixes = allocateMatrixes(size);
    for (i = 0; i < size; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            for (int k = 0; k < SIZE; k++)
            {
                matrixes[i][k][j] = 0;
                if (i * 16 + j * SIZE + k < input.length())
                {
                    matrixes[i][k][j] = input[i * 16 + j * SIZE + k];
                    
                }
                //std::cout << std::hex << (int)matrixes[i][k][j] << " ";

            }
        }
    }
    //std::cout << std::endl;
    return matrixes;
}

void AES::copyKey(unsigned char** src, unsigned char** dst)
{
    for (int i = 0; i < SIZE; ++i)
    {
        for (int j = 0; j < SIZE; ++j)
        {
            dst[i][j] = src[i][j];
        }
    }
}

unsigned char*** AES::allocateMatrixes(int size)
{
    unsigned char*** res = new unsigned char**[size];
    for (int i = 0; i < size; ++i)
    {
        res[i] = allocateMatrix(SIZE);
    }
    return res;
}

unsigned char** AES::allocateMatrix(int size)
{
    unsigned char** res = new unsigned char*[size];
    for (int j = 0; j < size; ++j)
    {
        res[j] = new unsigned char[size];
    }
    return res;
}

void AES::deallocateMatrixes(unsigned char*** msg, int size)
{
    for (int i = 0; i < size; ++i)
    {
        deallocateMatrix(msg[i], SIZE);
    }
    delete[] msg;
}

void AES::deallocateMatrix(unsigned char** msg, int size)
{
    for (int j = 0; j < size; ++j)
    {
        delete[] msg[j];
    }
    delete[] msg;
}

void AES::decryptBlock(unsigned char** state, unsigned char** pkey)
{
    int i = 1;
    unsigned char** key = allocateMatrix(SIZE);
    copyKey(pkey, key);
    unsigned char*** roundKeys = allocateMatrixes(10);
    for (i = 0; i < 10; ++i)
    {
        copyKey(key, roundKeys[i]);
        keyExpansion(key, i + 1);
    }
    AddRoundKey(state, key);
    for (i = 9; i > 0; i--)
    {
        InvShiftRows(state);
        InvSubBytes(state);
        AddRoundKey(state, roundKeys[i]);
        InvMixColumns(state);
    }
    InvShiftRows(state);
    InvSubBytes(state);
    AddRoundKey(state, roundKeys[i]);
    deallocateMatrix(key, SIZE);
    deallocateMatrixes(roundKeys, 10);
}

void AES::encryptBlock(unsigned char** state, unsigned char** pkey)
{
    int i = 1;
    AddRoundKey(state, pkey);
    unsigned char** key = allocateMatrix(SIZE);
    copyKey(pkey, key);
    unsigned char*** roundKeys = allocateMatrixes(10);
    for (i = 0; i < 10; i++)
    {
        keyExpansion(key, i + 1);
        copyKey(key, roundKeys[i]);
    }

    for (i = 0; i < 9; i++)
    {
        SubBytes(state);
        ShiftRows(state);
        MixColumns(state);
        AddRoundKey(state, roundKeys[i]);
    }
    SubBytes(state);
    ShiftRows(state);
    AddRoundKey(state, roundKeys[i]);
    deallocateMatrix(key, SIZE);
    deallocateMatrixes(roundKeys, 10);
}

void AES::keyExpansion(unsigned char** key, int round)
{
    unsigned char* expander = new unsigned char[4];
    for (int i = 0; i < SIZE; i++)
    {
        expander[i] = key[i][3];
    }
    char temp = expander[0];
    expander[0] = expander[1];
    expander[1] = expander[2];
    expander[2] = expander[3];
    expander[3] = temp;
    SubBytes(expander);
    //Adding round constants
    expander[0] ^= getRcon(round);
    for (int i = 0; i < SIZE; i++)
    {
        key[i][0] ^= expander[i];
    }
    for (int i = 0; i < 3; i++)
    {
        key[0][i + 1] ^= key[0][i];
        key[1][i + 1] ^= key[1][i];
        key[2][i + 1] ^= key[2][i];
        key[3][i + 1] ^= key[3][i];
    }
    delete[] expander;
    /*for (int i = 0; i < SIZE; i++)
    {
        for (int j = 0; j < SIZE; j++)
        {
            std::cout << std::hex << (int)key[j][i];
        }
    }
    std::cout << std::endl;*/

}

/*
*
*/
int AES::getRcon(int round)
{
    if (round < 1)
    {
        return -1;
    }
    if (round == 1)
    {
        return 1;
    }
    int prevRound = getRcon(round - 1) * 2;
    if (prevRound > 0x80)
    {
        prevRound ^= 0x11B;
    }
    return prevRound;
}

void AES::ShiftRow(unsigned char** state, unsigned int i, unsigned int n)  // shift row i on n positions
{
    unsigned char tmp[SIZE];
    for (unsigned int j = 0; j < SIZE; j++)
    {
        tmp[j] = state[i][(j + n) % SIZE];
    }
    memcpy(state[i], tmp, SIZE * sizeof(unsigned char));
}

void AES::ShiftRows(unsigned char** state)
{
    ShiftRow(state, 1, 1);
    ShiftRow(state, 2, 2);
    ShiftRow(state, 3, 3);
}

void AES::SubBytes(unsigned char* state)
{
    for (unsigned int i = 0; i < SIZE; i++)
    {
        state[i] = sbox[state[i] / HEX][state[i] % HEX];
    }
}

void AES::SubBytes(unsigned char** state)
{
    for (unsigned int i = 0; i < SIZE; i++)
    {
        for (unsigned int j = 0; j < SIZE; j++)
        {
            state[i][j] = sbox[state[i][j] / HEX][state[i][j] % HEX];
        }
    }
}

void AES::MixColumns(unsigned char** state)
{
    unsigned char temp_state[SIZE][SIZE];
    for (unsigned int i = 0; i < SIZE; ++i)
    {
        memset(temp_state[i], 0, SIZE);
    }
    for (int col = 0; col < SIZE; ++col)
    {
        for (int row = 0; row < SIZE; ++row)
        {
            int val = 0;
            for (int k = 0; k < SIZE; ++k) {
                val ^= aesMul(CMDS[row][k], state[k][col]);  // AES multiplication
            }
            temp_state[row][col] = val;
        }
    }
    for (unsigned int i = 0; i < SIZE; ++i) {
        memcpy(state[i], temp_state[i], 4);
    }
}

int AES::aesMul(int a, int b)
{
    int result = 0;
    while (b) {
        if (b & 1) {
            result ^= a;
        }
        int highBit = a & 0x80;
        a <<= 1;
        if (highBit) {
            a ^= 0x11B; // Reduction using AES irreducible polynomial
        }
        b >>= 1;
    }
    return result;
}



void AES::InvShiftRows(unsigned char** state)
{
    ShiftRow(state, 1, 3);
    ShiftRow(state, 2, 2);
    ShiftRow(state, 3, 1);
}

void AES::InvSubBytes(unsigned char** state)
{
    for (unsigned int i = 0; i < SIZE; i++)
    {
        for (unsigned int j = 0; j < SIZE; j++)
        {
            state[i][j] = inv_sbox[state[i][j] / HEX][state[i][j] % HEX];
        }
    }
}

void AES::InvMixColumns(unsigned char** state)
{
    unsigned char temp_state[SIZE][SIZE];
    for (unsigned int i = 0; i < SIZE; ++i)
    {
        memset(temp_state[i], 0, SIZE);
    }
    for (int col = 0; col < SIZE; ++col)
    {
        for (int row = 0; row < SIZE; ++row)
        {
            int val = 0;
            for (int k = 0; k < SIZE; ++k) {
                val ^= aesMul(INV_CMDS[row][k], state[k][col]);  // AES multiplication
            }
            temp_state[row][col] = val;
        }
    }
    for (unsigned int i = 0; i < SIZE; ++i) {
        memcpy(state[i], temp_state[i], 4);
    }

    /* unsigned char temp_state[SIZE][SIZE];

     for (unsigned int i = 0; i < SIZE; ++i)
     {
         memset(temp_state[i], 0, 4);
     }
     for (unsigned int i = 0; i < 4; ++i)
     {
         for (unsigned int k = 0; k < 4; ++k)
         {
             for (unsigned int j = 0; j < 4; ++j)
             {
                 temp_state[i][j] ^= GF_MUL_TABLE[INV_CMDS[i][k]][state[k][j]];
             }
         }
     }
     for (unsigned int i = 0; i < 4; ++i) {
         memcpy(state[i], temp_state[i], 4);
     }*/
}

void AES::AddRoundKey(unsigned char** state, unsigned char** key)
{
    for (unsigned int i = 0; i < SIZE; i++)
    {
        for (unsigned int j = 0; j < SIZE; j++)
        {
            state[i][j] = state[i][j] ^ key[i][j];
        }
    }
}


void AES::GKey()
{
    std::srand(static_cast<unsigned int>(std::time(0)));
    for (unsigned int i = 0; i < SIZE; i++)
    {
        for (unsigned int j = 0; j < SIZE; j++)
        {
            _key[i][j] = std::rand() % 127 + 1;
            
        }
    }
}
