#pragma once
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
#include <exception>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

#define SERVER_IP "127.0.0.1"
#define PORT 5078
class Peer
{
public:
	static std::map<int, SOCKET> users;
	static WSADATA wsaData;
	//Declarations
	static void recieveMessageAndHandle(SOCKET sc);
	static void connectMoreUsers(SOCKET listenSocket, int port);
	static int start();
private:
	


};

