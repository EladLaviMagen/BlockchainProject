#include "Peer.h"
//
int main()
{
	return Peer::start();
}

//{
//    int port = 0;
//    if (WSAStartup(MAKEWORD(2, 2), &Peer::wsaData) != 0) {
//        std::cerr << "WSAStartup failed\n";
//        return 1;
//    }
//    //Choosing option
//    std::string input = "";
//    std::cout << "Enter 'a' for server, anything else for client\n";
//    std::cin >> input;
//    //A socket both client and server use
//    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//    if (listenSocket == INVALID_SOCKET) {
//        std::cerr << "Error creating socket\n";
//        WSACleanup();
//        return 1;
//    }
//    if (input == "a")
//    {
//        //Starting listening for users
//        std::thread t(Peer::connectMoreUsers, listenSocket, PORT);
//        t.detach();
//    }
//    else
//    {
//        //Creating communication socket
//        SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
//        if (clientSocket == INVALID_SOCKET) {
//            std::cerr << "Error creating socket\n";
//            WSACleanup();
//            return 1;
//        }
//
//        sockaddr_in serverAddr;
//        serverAddr.sin_family = AF_INET;
//        serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
//        serverAddr.sin_port = htons(PORT);
//
//        //Connecting
//        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
//            int error = WSAGetLastError();
//            std::cerr << "Connection failed with error: " << error << "\n";
//            closesocket(clientSocket);
//            WSACleanup();
//            return 1;
//        }
//        //Setting position in map
//        Peer::users[PORT] = clientSocket;
//        std::cout << "Connected to " << PORT << "\n";
//        
//        //Client establishes his own port for others to approach him, he sends it to the person he connected to for him to inform the others
//        std::cout << "Enter port : ";
//        std::cin >> port;
//        Helper::sendData(clientSocket, "a" + Helper::getPaddedNumber(port, 5));
//        //Calling threads to handle messages and connections of other users
//        std::thread t(Peer::recieveMessageAndHandle, clientSocket);
//        t.detach();
//        std::thread t1(Peer::connectMoreUsers, listenSocket, port);
//        t1.detach();
//    }
//    input = "";
//    while (input != "exit")
//    {
//        while (Peer::users.empty())
//        {
//
//        }
//        std::getline(std::cin, input);
//        if (input[0] == '/')
//        {
//            int finalIndex = 1;
//            while (input[finalIndex] != '/')
//            {
//                finalIndex++;
//            }
//            int userPort = std::atoi(input.substr(1, finalIndex - 1).c_str());
//            input = std::to_string(port) + " --> " + input;
//            if (Peer::users.find(userPort) != Peer::users.end())
//            {
//                Helper::sendData(Peer::users[userPort], Helper::getPaddedNumber(input.length(), 5) + input);
//            }
//           
//        }
//        else
//        {
//            for (auto it = Peer::users.begin(); it != Peer::users.end(); it++)
//            {
//                Helper::sendData(it->second, Helper::getPaddedNumber(input.length(), 5) + input);
//            }
//        }
//        
//
//    }
//    // Handle communication with connected peer
//    for (auto it = Peer::users.begin(); it != Peer::users.end(); it++)
//    {
//        closesocket(it->second);
//    }
//    closesocket(listenSocket);
//    WSACleanup();
//    return 0;
//}
//
