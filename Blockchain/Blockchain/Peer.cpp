#include "Peer.h"

std::map<std::string, netInfo> Peer::users;
userInfo Peer::user;
std::string Peer::name;
WSADATA Peer::wsaData;

int Peer::start()
{
    Peer::user.net.port = PORT;
    Peer::name = "Boss";
    netInfo info;
    
    if (WSAStartup(MAKEWORD(2, 2), &Peer::wsaData) != 0) 
    {
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
        if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) 
        {
            //int error = WSAGetLastError();
            //std::cerr << "Connection failed with error: " << error << "\n";
            closesocket(clientSocket);
            //WSACleanup();
            throw std::exception();
        }
        //Setting position in map
        
        info.port = PORT;
        info.socket = clientSocket;
        Peer::users[Peer::name] = info;
        
        std::cout << "Connected to " << PORT << "\n";

        //Client establishes his own port for others to approach him, he sends it to the person he connected to for him to inform the others
        std::string port_str = "";
        while (!checkPort(port_str))
        {
            std::cout << "Enter port : ";
            std::getline(std::cin, port_str);
        }
        bool userFlag = false;
        std::pair<big, RSA> rsa;
        while (!userFlag)
        {
            userFlag = true;
            std::cout << "Username : ";
            std::getline(std::cin, Peer::name);
            for (int i = 0; i < name.length(); i++)
            {
                if (name[i] == '/')
                {
                    userFlag = false;
                    std::cout << "No '/' in usernames!\n";
                }
            }
            if (userFlag)
            {
                rsa = FileManager::loadRSA(name);
                if (rsa.first == -1)
                {
                    std::cout << "Such a user does not exist, are you creating a new user? Enter y/Y for yes, anything else for no\n";
                    std::string ver = "";
                    std::getline(std::cin, ver);
                    if (ver == "y" || ver == "Y")
                    {
                        rsa.second = RSA();
                        rsa.first = rsa.second.generatePublic();
                        FileManager::saveRSA(name, rsa.first, rsa.second);
                    }
                    else
                    {
                        userFlag = false;
                        std::cout << "Please re-enter username, as for user was not found\n";
                    }
                }
            }
            

        }

        Peer::user.net.port = std::stoi(port_str);
        Peer::user.e = rsa.first;
        Peer::user.p = rsa.second.getP();
        Peer::user.q = rsa.second.getQ();

        AES aes = AES(keyExchangeEntering(clientSocket));
        std::string sentName = aes.encrypt(name);
        std::string len = Helper::getPaddedNumber(sentName.length(), 5);
        Helper::sendData(clientSocket, aes.encrypt(port_str+'\n'+len)+ sentName);
        int length = Helper::getIntPartFromSocket(clientSocket, 10);
        user.chain = new Blockchain(Helper::getStringPartFromSocket(clientSocket, length));
        FileManager::save(user.chain->toString(), PATH + name + "Save.txt");
        //Calling threads to handle messages and connections of other users
        std::thread t(Peer::recieveMessageAndHandle, clientSocket);
        t.detach();
        std::thread t1(Peer::connectMoreUsers, listenSocket, user.net.port);
        t1.detach();
    }
    catch(std::exception e)
    {
        std::string chainStr = FileManager::load(PATH+name+"Save.txt");
        if (chainStr != "false")
        {
            user.chain = new Blockchain(chainStr);
           
        }
        else
        {
            user.chain = new Blockchain();
            FileManager::save(user.chain->toString(), PATH + name + "Save.txt");
        }
        
        
        


        std::pair<big, RSA> rsa = FileManager::loadRSA(Peer::name);
        if (rsa.first == -1)
        {
            RSA temp = RSA();

            Peer::user.e = temp.generatePublic();
            Peer::user.p = temp.getP();
            Peer::user.q = temp.getQ();
            FileManager::saveRSA(Peer::name, user.e, temp);
        }
        else 
        {
            Peer::user.e = rsa.first;
            Peer::user.p = rsa.second.getP();
            Peer::user.q = rsa.second.getQ();
        }
        //Starting listening for users
        std::thread t(Peer::connectMoreUsers, listenSocket, PORT);
        t.detach();
    }
    std::string input = "";
    while (input != "exit")
    {

        std::getline(std::cin, input);
        
        try
        {
            if (input[0] == '/')
            {
                std::vector<std::string> msg = FileManager::splitString(input, '/');
                input = Peer::name + " --> " + msg[2];
                if (Peer::users.find(msg[1]) != Peer::users.end())
                {
                    Helper::sendData(Peer::users[msg[1]].socket, Helper::getPaddedNumber(input.length(), 5) + input);
                }
            }
            else if (input[0] == '\\')
            {
                switch (input[1])
                {
                case 'm':
                    std::vector<std::string> commandDetails = FileManager::splitString(input, ' ');
                    std::cout << "User " << commandDetails[1] << " Has " << user.chain->getCoinsOf(commandDetails[1]) << " coins\n";
                    break;
                    
                }
            }
            else
            {
                if (!Peer::users.empty())
                {
                    for (auto it = Peer::users.begin(); it != Peer::users.end(); it++)
                    {
                        if (input != "exit")
                        {
                            input = Peer::name + " : " + input;
                        }
                        Helper::sendData(it->second.socket, Helper::getPaddedNumber(input.length(), 5) + input);
                    }
                }
                
            }
        }
        catch (std::exception e)
        {
            std::cout << "There was an error with the input, please retry\n";
        }
        


    }

    if (Peer::users.begin() != Peer::users.end())
    {
        for (auto it = Peer::users.begin(); it != Peer::users.end(); it++)
        {
            closesocket(it->second.socket);
        }
    }
    // Handle communication with connected peer
    
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

std::string Peer::keyExchangeRecieving(SOCKET clientSocket)
{
    //Getting RSA parts
    std::string pubKey = Helper::getStringPartFromSocket(clientSocket, BIG_SIZE);
    std::string p = Helper::getStringPartFromSocket(clientSocket, BIG_SIZE);
    std::string q = Helper::getStringPartFromSocket(clientSocket, BIG_SIZE);
    RSA rsa = RSA(std::stoll(p), std::stoll(q));
    //Preparing AES key and encrypting it using RSA
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
        //Sending it to the user, letter by letter
        Helper::sendData(clientSocket, Helper::getPaddedNumber(encryptedKey[i], BIG_SIZE));
    }
    return key;
}

std::string Peer::keyExchangeEntering(SOCKET clientSocket)
{
    RSA rsa = RSA(user.p, user.q);
    big e = user.e;
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
    return key_str;
}

void Peer::sendDetails(SOCKET clientSocket)
{
    std::string details = Helper::getPaddedNumber(user.net.port, 5);
    details += '/' + Peer::name;
    Helper::sendData(clientSocket, "b" + Helper::getPaddedNumber(details.length(), 5) + details);
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
            //Getting length of message
            int length = Helper::getIntPartFromSocket(sc, 5);
            //Getting the rest of the message
            std::string msg = Helper::getStringPartFromSocket(sc, length);
            //Checking for special message 
            if (msg[0] == '\n')
            {
                //This is the handling of a special message, the special message is a new user that has connected
                //Getting the port of the new user (it's indexes in the message)
                std::vector<std::string> details = FileManager::splitString(msg, '\t');
                //New communication socket
                SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
                if (clientSocket == INVALID_SOCKET) {
                    std::cerr << "Error creating socket\n";
                    //WSACleanup();
                }

                sockaddr_in serverAddr;
                serverAddr.sin_family = AF_INET;
                serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
                //Setting port
                serverAddr.sin_port = htons(std::stoi(details[1]));

                //Connecting
                if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
                    int error = WSAGetLastError();
                    std::cerr << "Connection failed with error: " << error << "\n";
                    closesocket(clientSocket);
                    //WSACleanup();
                }
                sendDetails(clientSocket);
                
                //Assigning position in map
                netInfo info;
                info.port = std::stoi(details[1]);
                info.socket = clientSocket;
                users[details[1]] = info;
                std::thread t(recieveMessageAndHandle, clientSocket);
                t.detach();
            }
            else if (msg == "exit")
            {
                for (auto it = users.begin(); it != users.end(); it++)
                {
                    if (it->second.socket == sc)
                    {
                        std::cout << "User " << it->first << " disconnected\n";
                        users.erase(it->first);
                        break;
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
            if (it->second.socket == sc)
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
                //closesocket(listenSocket);
                //WSACleanup();
            }
            //Getting his details
            std::string flag = Helper::getStringPartFromSocket(clientSocket, 1);
            std::string name = "";
            netInfo info;
            info.socket = clientSocket;
            //Setting up message to inform others of the new user
            if (NEW_USER == flag)
            {
                int port = 0;
                AES aes = AES(keyExchangeRecieving(clientSocket));
                std::string details_str = Helper::getStringPartFromSocket(clientSocket, HEX);
                details_str = aes.decrypt(details_str);
                std::vector<std::string> details = FileManager::splitString(details_str, '\n');
                port = std::stoi(details[0]);
                name = Helper::getStringPartFromSocket(clientSocket, std::stoi(details[1]));
                name = aes.decrypt(name);
                name = AES::trimString(name);
                std::string enterMsg = "\n" + name + '\t' + std::to_string(port) + '\t' + "Entered\n";
                std::cout << enterMsg;
                std::string curChain = user.chain->toString();
                curChain = Helper::getPaddedNumber(curChain.length(), 10) + curChain;
                Helper::sendData(clientSocket, curChain);
                info.port = port;
                //Sending it to all other users
                if (!users.empty())
                {
                    enterMsg = Helper::getPaddedNumber(enterMsg.length(), 5) + enterMsg;
                    for (auto it = users.begin(); it != users.end(); it++)
                    {
                        Helper::sendData(it->second.socket, enterMsg);
                    }
                }
            }
            else
            {
                int length = Helper::getIntPartFromSocket(clientSocket, 5);
                std::string msg = Helper::getStringPartFromSocket(clientSocket, length);
                std::vector<std::string> details = FileManager::splitString(msg, '/');
                info.port = std::stoi(details[0]);
                name = details[1];
            }
            //Adding new user to the map
            users[name] = info;
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
