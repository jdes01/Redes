#ifndef SERVER_H
#define SERVER_H

#define BUFFER_SIZE 128
#define MAX_CLIENTS 30

// select() and fd_set
#include <sys/select.h>

// socket() and macros
#include <sys/types.h>
#include <sys/socket.h>

// sockaddr_in structure
#include <netinet/in.h>

// htons(), inet_addr()
#include <arpa/inet.h>

// close()
#include <unistd.h>

// bzero()
#include <cstring>

// Threads
#include <future>
#include <vector>


// String
#include <string>
using std::string;

// STL Data Types
#include <vector>
#include <algorithm>
using std::vector;
using std::size_t;

#include <iostream>
using std::cout;
using std::endl;

#include <regex>

#include <fstream>
using std::ofstream;

// Game Classes
#include "FillMissingLettersGame.cc"

#include "User.h"

class Server {

    private:
        int serverSocketDescriptor_;
        int newClientSocketDescriptor_;

        int numberOfClients_;
        
        struct sockaddr_in serverSocketData_;   // tipo de direccion, puerto y direccion IP
        struct sockaddr_in clientSocketData_;   // tipo de direccion, puerto y direccion IP
        
        socklen_t clientSocketDataSize_;
        
        char messageBuffer_[BUFFER_SIZE];
        
        vector <User> usersConnected_;
        
        fd_set fileDescriptorSet_;
        fd_set auxiliarFileDescriptor_;
        

        std::vector<std::future<int>> mThreads;
        std::vector<FillMissingLettersGame> mMatches;

        // startServer
        void recreateFileDescriptor_();
        void handleNewClient_();
        void addClientToServer_(int newClientSocketDescriptor);
        void sendTooManyClientsMessageToNewClient_();
        void exitClient_(int clientSocketDescriptor);

        void clientMessageHandler_(User &user, const char* message);
        
        void searchMatchForClient_(User &user);
        void serverMessageHandler_();
        void closeServer_();

        bool isClientLogged(int clientSocketDescriptor);

        void registerUser(string userName, string password);

        int logInClient(int clientSocketDescriptor);

        int registerOrLoginProcess(int newClientSocketDescriptor_); //1 success 0 error

        void addUserToFile(User user);

        bool checkIfUserIsWritten(string userName, string password);

    public:
        Server(int serverPort);
        void startServer();
};   


#endif // SERVER_H 