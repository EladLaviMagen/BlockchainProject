#include "AES.h"
#include <string>


int main()
{
    AES* cipher = new AES();
    std::string msg = "";
    std::getline(std::cin, msg);
    while (msg != "exit")
    {
        msg = cipher->encrypt(msg);
        for (int i = 0; i < msg.length(); i++)
        {
            std::cout << std::hex << (int)((char)msg[i]) << " ";
        }
        std::cout << std::endl;
        msg = cipher->decrypt(msg);
        std::cout << msg << std::endl;
        msg = "";
        std::getline(std::cin, msg);
    }
    delete cipher;

    return 0;

}



//void MixColumns(unsigned char state[SIZE][SIZE])
//{
//    unsigned char temp_state[SIZE][SIZE];
//
//    for (unsigned int i = 0; i < SIZE; ++i)
//    {
//        memset(temp_state[i], 0, 4);
//    }
//    for (unsigned int i = 0; i < 4; ++i)
//    {
//        for (unsigned int k = 0; k < 4; ++k)
//        {
//            for (unsigned int j = 0; j < 4; ++j)
//            {
//                if (CMDS[i][k] == 1)
//                    temp_state[i][j] ^= state[k][j];
//                else
//                    temp_state[i][j] ^= GF_MUL_TABLE[CMDS[i][k]][state[k][j]];
//            }
//        }
//    }
//
//    for (unsigned int i = 0; i < 4; ++i) {
//        memcpy(state[i], temp_state[i], 4);
//    }
//
//}

