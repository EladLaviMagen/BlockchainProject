#include "Peer.h"

std::map<std::string, netInfo> Peer::users;
std::vector<Transaction*> Peer::queue;
userInfo Peer::user;
std::string Peer::name;
WSADATA Peer::wsaData;
std::mutex Peer::lock_users;

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
        std::string check = Helper::getStringPartFromSocket(clientSocket, 3);
        if (check == ACCEPTED)
        {
            int length = Helper::getIntPartFromSocket(clientSocket, 10);
            user.chain = new Blockchain(Helper::getStringPartFromSocket(clientSocket, length));
            FileManager::save(user.chain->toString(), PATH + name + "Save.txt");
            //Calling threads to handle messages and connections of other users
            std::thread t(Peer::recieveMessageAndHandle, clientSocket);
            t.detach();
            std::thread t1(Peer::connectMoreUsers, listenSocket, user.net.port);
            t1.detach();
        }
        else
        {
            std::cout << "User already logged in!\n";
            std::cout << "Press any key to exit\n";
            getchar();
            return 1;
        }
        
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
        
        
        std::cout << user.chain->verifiy() << std::endl;


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
                std::vector<std::string> commandDetails = FileManager::splitString(input, ' ');
                switch (input[1])
                {
                case 'g':
                    std::cout << "User " << commandDetails[1] << " Has " << user.chain->getCoinsOf(commandDetails[1]) << " coins\n";
                    break;
                case 't':
                    std::cout << "Are you sure that you want to transfer " << commandDetails[1] << " to " << commandDetails[2] << std::endl;
                    std::cout << "Enter Y/y for yes and anything else for no : ";
                    std::getline(std::cin, input);
                    if (input == "Y" || input == "y")
                    {
                        if (user.chain->getCoinsOf(name) >= std::stof(commandDetails[1]) && std::stof(commandDetails[1]) > 0)
                        {
                            if (commandDetails[2] != name)
                            {
                                RSA cipher = RSA(user.p, user.q);
                                big* enc = new big[3];
                                enc[KEY] = cipher.modInverse(user.e);
                                enc[RSA_P] = user.p;
                                enc[RSA_Q] = user.q;
                                Transaction* t = new Transaction(std::stoi(commandDetails[1]), name, commandDetails[2], enc);
                                input = t->toString();
                                sendToAllUsers(TRANSACTION, input);
                                user.chain->addTransaction(t);
                                FileManager::save(user.chain->toString(), PATH + name + "Save.txt");
                                delete[] enc;
                            }
                            }
                            
                        else
                        {
                            std::cout << "You don't have coins for this transactions\n";
                        }
                        
                    }
                    break;
                case 'h':
                    user.chain->printHistory();
                    break;
                case 'c':
                    user.chain->printCurTransactions();
                    break;
                case 's':
                    if (!users.empty())
                    {
                        for (auto it = users.begin(); it != users.end(); it++)
                        {
                            std::cout << "- " << it->first << std::endl;
                        }
                    }
                    break;
                case 'm':
                    Blockchain::mining = !Blockchain::mining;
                    if (Blockchain::mining)
                    {
                        std::thread miner(mineCurBlock);
                        miner.detach();
                    }
                    else
                    {
                        for (int i = 0; i < queue.size(); i++)
                        {
                            user.chain->addTransaction(queue[i]);
                        }
                        FileManager::save(user.chain->toString(), PATH + name + "Save.txt");
                        Peer::queue.clear();
                    }
                    break;
                default:
                    std::cout << "Indentified command! Please retry!\n";
                }
            }
            else
            {
                if (input != "exit")
                {
                    input = Peer::name + " : " + input;
                }
                sendToAllUsers(MSG, input);
            }
        }
        catch (std::exception e)
        {
            std::cout << "There was an error with the input, please retry\n";
        }
        


    }

    if (Peer::users.begin() != Peer::users.end())
    {
        std::lock_guard<std::mutex> lock(Peer::lock_users);
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


std::string Peer::getNameOfUser(SOCKET sc)
{
    std::lock_guard<std::mutex> lock(Peer::lock_users);
    for (auto it = users.begin(); it != users.end(); it++)
    {
        if (it->second.socket == sc)
        {
            return it->first;
        }
    }
}

void Peer::sendToAllUsers(std::string code, std::string input)
{
    std::lock_guard<std::mutex> lock(Peer::lock_users);
    if (input == "exit")
    {
        code = LOGOUT;
    }
    if (!Peer::users.empty())
    {
        for (auto it = Peer::users.begin(); it != Peer::users.end(); it++)
        {
            
            Helper::sendData(it->second.socket, code + Helper::getPaddedNumber(input.length(), 5) + input);
        }
    }
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

void Peer::mineCurBlock()
{

    sendToAllUsers(MINE_START, "");
    std::cout << "Mining started!" << std::endl;
    while (Blockchain::mining)
    {
        std::string results = user.chain->mine(name);
        if (results != "failed")
        {
            Blockchain::mining = false;
            sendToAllUsers(NEWBLOCK, results);
            std::cout << "Mining successful!" << std::endl;
            FileManager::save(user.chain->toString(), PATH + name + "Save.txt");
            return;
        }
    }
    sendToAllUsers(MINE_CANCEL, "");
    std::cout << "Mining cancalled!" << std::endl;
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
            std::string code = Helper::getStringPartFromSocket(sc, 3);
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
            else if (code == LOGOUT)
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
            else if (code == TRANSACTION)
            {
                bool passed = false;;
                Transaction* t = new Transaction(msg);
                if (t->getSender() == getNameOfUser(sc))
                {
                    if (t->getSender() != t->getRecv())
                    {
                        std::pair<long long, RSA> ciphers = FileManager::loadRSA(t->getSender());
                        big* nums = new big[3];
                        nums[RSA_P] = ciphers.second.getP();
                        nums[RSA_Q] = ciphers.second.getQ();
                        nums[KEY] = ciphers.first;
                        if (t->verify(nums) == VERIFIED)
                        {
                            passed = true;
                            msg = t->getSender() + " wants to send " + std::to_string(t->getSum()) + " coins to " + t->getRecv();
                            if (Blockchain::mining)
                            {
                                std::cout << "NEW TRANSACTIONS - RESTART MINING TO INCLUDE THEM IN THIS BLOCK\n";
                                queue.push_back(t);
                            }
                            else
                            {
                                if (user.chain->addTransaction(t))
                                {
                                    FileManager::save(user.chain->toString(), PATH + name + "Save.txt");
                                }
                                else
                                {
                                    std::cout << "BLOCK FULL - MINE IT TO START NEW BLOCK\n";
                                }

                            }
                        }
                    }
                   
                }
                if (!passed)
                {
                    delete t;
                    msg = "UNAUTHORIZED TRANSACTION DETECTED, NOT INSERTED INTO CHAIN";
                }
               
                
                
            }
            else if (code == NEWBLOCK)
            {
                if (Blockchain::mining)
                {
                    Blockchain::mining = false;
                }
                std::vector<std::string> mineData = FileManager::splitString(msg, CHAIN_DELIM);
                Block* newBlock = new Block(mineData[0]);
                user.chain->addBlock(newBlock, mineData[1]);
                user.chain->update(mineData[2]);
                FileManager::save(user.chain->toString(), PATH + name + "Save.txt");
                msg = "Block was mined!\n";
                
            }
            else if (code == MINE_START)
            {
                std::cout << getNameOfUser(sc) << " started mining\n";
            }
            else if (code == MINE_CANCEL)
            {
                std::cout << getNameOfUser(sc) << " stopped mining\n";
            }
            //Displaying message
            std::cout << msg << std::endl;
        }
    }
    catch (std::exception ex)
    {
        std::lock_guard<std::mutex> lock(lock_users);
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
                if (name == Peer::name)
                {
                    Helper::sendData(clientSocket, DENIED);
                    std::cout << "User already logged in!\n";
                    throw(std::exception());
                }
                if (!users.empty())
                {
                    std::lock_guard<std::mutex> lock(lock_users);
                    for (auto it = users.begin(); it != users.end(); it++)
                    {
                        if (it->first == name)
                        {
                            Helper::sendData(clientSocket, DENIED);
                            std::cout << "User already logged in!\n";
                            throw(std::exception());
                        } 
                    }
                    
                }
                std::string enterMsg = "\n" + name + '\t' + std::to_string(port) + '\t' + "Entered\n";
                std::cout << enterMsg;
                std::string curChain = user.chain->toString();
                curChain = Helper::getPaddedNumber(curChain.length(), 10) + curChain;
                Helper::sendData(clientSocket, ACCEPTED + curChain);
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
            {
                std::lock_guard<std::mutex> lock(lock_users);
                users[name] = info;
            }
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
