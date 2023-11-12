#include <exception>
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <fstream>
#pragma warning(disable: 4996)

#define PORT 5078

int main()
{
	std::string input = "";
	std::cout << "Would you like to open a network?\n";
	std::cin >> input;
	SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");
	if (input == "a")
	{
		

		

		struct sockaddr_in sa = { 0 };

		sa.sin_port = htons(PORT); // port that server will listen for
		sa.sin_family = AF_INET;   // must be AF_INET
		sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

		// Connects between the socket and the configuration (port and etc..)
		if (bind(sock, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
			throw std::exception(__FUNCTION__ " - bind");

		// Start listening for incoming requests of clients
		if (listen(sock, SOMAXCONN) == SOCKET_ERROR)
			throw std::exception(__FUNCTION__ " - listen");
		std::cout << "Listening on port " << PORT << std::endl;

		// this accepts the client and create a specific socket from server to this client
			// the process will not continue until a client connects to the server
		SOCKET client_socket = accept(sock, NULL, NULL);
		if (client_socket == INVALID_SOCKET)
			throw std::exception(__FUNCTION__);

		std::cout << "Client accepted. Server and client can speak" << std::endl;

		while (true)
		{

			//m_clients[client_socket] = handler;
		}
	}
	else
	{
		input = "";
		std::cout << "enter IP of network\n";
		std::cin >> input;
		struct sockaddr_in sa = { 0 };
		sa.sin_port = htons(PORT); // port that server will listen for 
		sa.sin_family = AF_INET;   // must be AF_INET 
		sa.sin_addr.s_addr = inet_addr(input.c_str());
		connect(sock, (struct sockaddr*)&sa, sizeof(sa));

	}
	
    return 0;
}