#pragma once
#include <iostream>
#include <string>
#include "Blockchain.h"

struct netInfo
{
	SOCKET socket;
	int port;
};

struct userInfo
{
	Blockchain* chain;
	netInfo net;
	long long e;
	long long p;
	long long q;
};

#define DENIED "999"
#define ACCEPTED "111"
#define MSG "900"
#define PULSE "100"
#define LOGIN "200"
#define ACCEPT "225"
#define LOGOUT "250"
#define UPDATE "300"
#define TRANSACTION "325"
#define NEWBLOCK "350"
#define MINE_START "500"
#define MINE_CANCEL "600"

#define PARAM_PORT 0
#define PARAM_NAME 1

