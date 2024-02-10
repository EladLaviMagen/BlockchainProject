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
#include "AES.h"
#include "RSA.h"
#include <map>
#include <exception>
#pragma comment(lib, "ws2_32.lib")
#pragma warning(disable: 4996)

#define SERVER_IP "127.0.0.1"
#define PORT 5078

#define NEW_USER "a"
#define CONNECTING "b"
#define PORT_SIZE 5
#define BIG_SIZE 15
class Peer
{
public:
	static std::map<int, SOCKET> users;
	
	//Declarations
	static void recieveMessageAndHandle(SOCKET sc);
	static void connectMoreUsers(SOCKET listenSocket, int port);
	static int start();
private:
	static WSADATA wsaData;
	static bool checkPort(std::string port);
	static bool checkNum(std::string num);


};

