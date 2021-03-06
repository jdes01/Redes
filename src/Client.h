#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>

// sockaddr_in
#include <netinet/in.h>

// close()
#include <unistd.h>

// htons() and inet_addr()
#include <arpa/inet.h>

#include <string>
#include <string.h>
using std::string;

#include <iostream>
using std::cout;
using std::endl;


class Client {

    int clientSocketDescriptor_;            // integer (-1 if something went wrong)
    
    struct sockaddr_in serverSocketData_;   // tipo de direccion, puerto y direccion IP
    fd_set fileDescriptorSet;               // set de file descriptors
    char messageBuffer_[128];               // buffer de la funcion recvfrom
    bool endCommunication_;                 // Communication flag

    private:
        void openClientSocket_();
        void setServerSocketDataStructure_(string serverIpAddress, int serverPortNumber);
        void requestServerConnection_();
        void setFileDescriptorStructures_();

        void readServerMessage_();
        void handleServerErrorMessages_();
        void sendMessageToServer_();
        void handleClientMessage_();

    public:
        Client(string serverIpAddress, int serverPortNumber);

        void startCommunication();
        inline void closeClient() { close(clientSocketDescriptor_); }
};   

#endif