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

// String
#include <string>
using std::string;

// STL Data Types
#include <vector>
using std::vector;
using std::size_t;

#include <iostream>
using std::cout;
using std::endl;

// Pasapalabra Classes
#include "PasaPalabraGameHandler.h"
#include "PasaPalabraGame.h"
#include "Player.h"

class Server {

    private:
        int serverSocketDescriptor_;
        int newClientSocketDescriptor_;

        int serverCapacity_;
        
        struct sockaddr_in serverSocketData_;   // tipo de direccion, puerto y direccion IP
        struct sockaddr_in clientSocketData_;   // tipo de direccion, puerto y direccion IP
        
        socklen_t clientSocketDataSize_;
        
        char messageBuffer_[BUFFER_SIZE];
        
        //vector <PasaPalabraGameHandler> Games_;
        vector <std::future <int> > Threads_;

        vector <int> clientsConnected_;
        vector <int> playersQueue_;
        
        fd_set readerFileDescriptor_;
        fd_set auxiliarFileDescriptor_;
        

        // startServer
        void recreateFileDescriptor_();
        void handleNewClient_();
        void addClientToServer_();
        void sendTooManyClientsMessageToNewClient_();
        void exitClient_(int clientSocketDescriptor);
        void clientMessageHandler_(int socketID);
        void searchMatchForClient_(int clientSocketDescriptor);
        void sendMessageBufferToAllPlayers_(vector <int> gamePlayers);
        void createDominoGame_(vector <int> gamePlayers);
        void eraseClientsReadyForGame_(vector <int> gamePlayers);
        void serverMessageHandler_();
        void closeServer_();

    public:
        Server(int serverPort);
        void startServer();
        void addClientsToServer(vector <int> clientsToAdd);
};   


#endif // SERVER_H 