#include "Peer.h"

std::map<int, SOCKET> Peer::users;
WSADATA Peer::wsaData;

int Peer::start()
{
    int port = 0;
    if (WSAStartup(MAKEWORD(2, 2), &Peer::wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }
    //A socket both client and server use
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket\n";
        WSACleanup();
        return 1;
    }
    try
    {
        //Creating communication socket
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

        //Connecting
        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            //int error = WSAGetLastError();
            //std::cerr << "Connection failed with error: " << error << "\n";
            closesocket(clientSocket);
            //WSACleanup();
            throw std::exception();
        }
        //Setting position in map
        Peer::users[PORT] = clientSocket;
        std::cout << "Connected to " << PORT << "\n";

        //Client establishes his own port for others to approach him, he sends it to the person he connected to for him to inform the others
        std::string port_str = "";
        while (!checkPort(port_str))
        {
            std::cout << "Enter port : ";
            std::getline(std::cin, port_str);
        }
        port = std::stoi(port_str);
        RSA rsa = RSA();
        big e = rsa.generatePublic();
        Helper::sendData(clientSocket, "a" + Helper::getPaddedNumber(e, BIG_SIZE));
        Helper::sendData(clientSocket, Helper::getPaddedNumber(rsa.getP(), BIG_SIZE));
        Helper::sendData(clientSocket, Helper::getPaddedNumber(rsa.getQ(), BIG_SIZE));

        longString aes_key;
        
        for (int i = 0; i < HEX; i++)
        {
            aes_key.push_back(Helper::getBigPartFromSocket(clientSocket, BIG_SIZE));
        }
        aes_key = rsa.rsaMain(aes_key, rsa.modInverse(e));
        std::string key_str = "";
        for (int i = 0; i < HEX; i++)
        {
            key_str += (char)aes_key[i];
        }
        AES aes = AES(key_str);
        Helper::sendData(clientSocket, aes.encrypt(port_str));
        //Calling threads to handle messages and connections of other users
        std::thread t(Peer::recieveMessageAndHandle, clientSocket);
        t.detach();
        std::thread t1(Peer::connectMoreUsers, listenSocket, port);
        t1.detach();
    }
    catch(std::exception e)
    {
        //Starting listening for users
        std::thread t(Peer::connectMoreUsers, listenSocket, PORT);
        t.detach();
    }
    std::string input = "";
    while (input != "exit")
    {
        while (Peer::users.empty())
        {

        }
        std::getline(std::cin, input);
        if (input[0] == '/')
        {
            int finalIndex = 1;
            while (input[finalIndex] != '/')
            {
                finalIndex++;
            }
            int userPort = std::atoi(input.substr(1, finalIndex - 1).c_str());
            input = std::to_string(userPort) + " --> " + input.substr(finalIndex + 1, input.length() - 1);
            if (Peer::users.find(userPort) != Peer::users.end())
            {
                Helper::sendData(Peer::users[userPort], Helper::getPaddedNumber(input.length(), 5) + input);
            }

        }
        else
        {
            for (auto it = Peer::users.begin(); it != Peer::users.end(); it++)
            {
                Helper::sendData(it->second, Helper::getPaddedNumber(input.length(), 5) + input);
            }
        }


    }
    // Handle communication with connected peer
    for (auto it = Peer::users.begin(); it != Peer::users.end(); it++)
    {
        closesocket(it->second);
    }
    closesocket(listenSocket);
    WSACleanup();
    return 0;
}

bool Peer::checkPort(std::string port)
{
    if (port.size() <= 5 && port.size() > 0)
    {
        return checkNum(port);
    }
    return false;
}

bool Peer::checkNum(std::string num)
{
    for (int i = 0; i < num.size(); i++)
    {
        if (num[i] < '0' || num[i] > '9')
        {
            return false;
        }
    }
    return true;
}


/*
* Function will recieve a message and handle it accordingly
* Input : Socket to get message from
* Output : None
*/
void Peer::recieveMessageAndHandle(SOCKET sc)
{
    //Forever listening
    try
    {
        while (true)
        {
            int port = 0;
            //Getting length of message
            int length = Helper::getIntPartFromSocket(sc, 5);
            //Getting the rest of the message
            std::string msg = Helper::getStringPartFromSocket(sc, length);
            //Checking for special message 
            if (msg[0] == '\n')
            {
                //This is the handling of a special message, the special message is a new user that has connected
                //Getting the port of the new user (it's indexes in the message)
                int finalIndex = 6;
                while (msg[finalIndex] != ' ')
                {
                    finalIndex++;
                }
                //New communication socket
                SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (clientSocket == INVALID_SOCKET) {
                    std::cerr << "Error creating socket\n";
                    WSACleanup();
                }

                sockaddr_in serverAddr;
                serverAddr.sin_family = AF_INET;
                serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
                //Setting port
                serverAddr.sin_port = htons(std::atoi(msg.substr(6, finalIndex - 1).c_str()));

                //Connecting
                if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
                    int error = WSAGetLastError();
                    std::cerr << "Connection failed with error: " << error << "\n";
                    closesocket(clientSocket);
                    WSACleanup();
                }
                Helper::sendData(clientSocket, "b" + Helper::getPaddedNumber(port, 5));
                //Assigning position in map
                users[std::atoi(msg.substr(6, finalIndex - 1).c_str())] = clientSocket;
                std::thread t(recieveMessageAndHandle, clientSocket);
                t.detach();
            }
            else if (msg == "exit")
            {
                for (auto it = users.begin(); it != users.end(); it++)
                {
                    if (it->second == sc)
                    {
                        std::cout << "User " << it->first << " disconnected\n";
                        users.erase(it->first);
                    }
                }
                closesocket(sc);
                return;
            }
            //Displaying message
            std::cout << msg << std::endl;
        }
    }
    catch (std::exception ex)
    {
        for (auto it = users.begin(); it != users.end(); it++)
        {
            if (it->second == sc)
            {
                std::cout << "User " << it->first << " disconnected\n";
                users.erase(it->first);
                break;
            }
        }
    }
    closesocket(sc);
}

/*
* Function will connect incoming users
* Input : Listen socket and port
* Output : None
*/
void Peer::connectMoreUsers(SOCKET listenSocket, int port)
{

    //Setting socket
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    //Binding and listening to incoming connections
    if (bind(listenSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed\n";
        closesocket(listenSocket);
        WSACleanup();
    }

    if (listenSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket\n";
        WSACleanup();
    }

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed\n";
        closesocket(listenSocket);
        WSACleanup();
    }

    //Forever listening

    while (true)
    {
        std::cout << "Waiting for incoming connections...\n";
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);
        //Accepting new client
        try
        {
            SOCKET clientSocket = accept(listenSocket, (struct sockaddr*)&clientAddr, &addrLen);
            if (clientSocket == INVALID_SOCKET) {
                std::cerr << "Accept failed\n";
                closesocket(listenSocket);
                WSACleanup();
            }
            //Getting his port
            std::string flag = Helper::getStringPartFromSocket(clientSocket, 1);
            int port = 0;
            //Setting up message to inform others of the new user
            if (NEW_USER == flag)
            {
                std::string pubKey = Helper::getStringPartFromSocket(clientSocket, BIG_SIZE);
                std::string p = Helper::getStringPartFromSocket(clientSocket, BIG_SIZE);
                std::string q = Helper::getStringPartFromSocket(clientSocket, BIG_SIZE);
                RSA rsa = RSA();
                rsa.setQandP(std::stoll(q), std::stoll(p));
                AES aes = AES();
                std::string key = aes.getKey();
                longString encryptedKey;
                for (int i = 0; i < HEX; i++)
                {
                    encryptedKey.push_back(key[i]);
                }
                encryptedKey = rsa.rsaMain(encryptedKey, std::stoll(pubKey));
                for (int i = 0; i < HEX; i++)
                {
                    Helper::sendData(clientSocket, std::to_string(encryptedKey[i]));
                }
                std::string port_str = Helper::getStringPartFromSocket(clientSocket, HEX);
                port_str = aes.decrypt(port_str);
                port = std::stoi(port_str);
                std::string enterMsg = "\nUser " + std::to_string(port) + " Entered\n";
                std::cout << enterMsg;
                //Sending it to all other users
                if (!users.empty())
                {
                    enterMsg = Helper::getPaddedNumber(enterMsg.length(), 5) + enterMsg;
                    for (auto it = users.begin(); it != users.end(); it++)
                    {
                        Helper::sendData(it->second, enterMsg);
                    }
                }
            }
            else
            {
                port = Helper::getIntPartFromSocket(clientSocket, 5);
            }

            //Adding new user to the map
            users[port] = clientSocket;
            //Listening to messages from him
            std::thread t(recieveMessageAndHandle, clientSocket);
            t.detach();
        }
        catch (std::exception ex)
        {
            std::cout << "Had trouble connecting user\n";
        }
    }

}
