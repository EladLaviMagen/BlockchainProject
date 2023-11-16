#include <exception>
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <thread>
#include "Helper.h"
#include <map>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

#define SERVER_IP "127.0.0.1"
#define PORT 5078


void recieveMessage(SOCKET sc);
void connectMoreUsers(SOCKET listenSocket, int port);
std::map<int, SOCKET> users;
int main() 
{
    
    std::string input = "";
    std::cout << "Enter 'a' for server, anything else for client\n";
    std::cin >> input;
    
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (input == "a")
    {
        std::thread t(connectMoreUsers, listenSocket, PORT);
        t.detach();
        input = "";
        while (input != "exit")
        {
            while (users.empty())
            {

            }
            std::cin >> input;
            for (auto it = users.begin(); it != users.end(); it++)
            {
                Helper::sendData(it->second, input);
            }
           
        }
        // Handle communication with connected peer
        for (auto it = users.begin(); it != users.end(); it++)
        {
            closesocket(it->second);
        }
        closesocket(listenSocket);
    }
    else
    {
       
        SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error creating socket\n";
            WSACleanup();
            return 1;
        }

        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
        serverAddr.sin_port = htons(PORT);

        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            int error = WSAGetLastError();
            std::cerr << "Connection failed with error: " << error << "\n";
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
        users[PORT] = clientSocket;
        std::cout << "Connected to " << PORT << "\n";
        int port = 0;
        std::cout << "Enter port : ";
        std::cin >> port;
        std::thread t(connectMoreUsers, listenSocket, port);
        input = "";
        while (input != "exit")
        {
            std::cin >> input;
            for (auto it = users.begin(); it != users.end(); it++)
            {
                Helper::sendData(it->second, input);
            }
        }
        // Handle communication with the server
        for (auto it = users.begin(); it != users.end(); it++)
        {
            closesocket(it->second);
        } 

    }
    WSACleanup();
    return 0;
}


void recieveMessage(SOCKET sc)
{
    while (true)
    {
        std::string msg = Helper::getStringPartFromSocket(sc, 30);
        std::cout << msg << std::endl;
    } 
}

void connectMoreUsers(SOCKET listenSocket, int port)
{
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(listenSocket);
        WSACleanup();
    }

    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket\n";
        WSACleanup();
    }

    while (true)
    {
        if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
            std::cerr << "Listen failed\n";
            closesocket(listenSocket);
            WSACleanup();
        }

        std::cout << "Waiting for incoming connections...\n";
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Accept failed\n";
            closesocket(listenSocket);
            WSACleanup();
        }

        int port = Helper::getIntPartFromSocket(clientSocket, 5);
        std::cout << "Peer " << port << " connected\n";
        users[port] = clientSocket;
        std::thread t(recieveMessage, clientSocket);
        t.detach();
    }
}