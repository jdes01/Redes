#include "Client.h"

/*
 * Client constructor, will be summoned in client main program
*/

Client::Client(string serverIpAddress, int serverPortNumber){

    openClientSocket_();
    setServerSocketDataStructure_(serverIpAddress, serverPortNumber);
    requestServerConnection_();
    endComunication_ = false;
}

/*
 * open client socket method; calls the socket and check for errors
*/

void Client::openClientSocket_(){

    clientSocketDescriptor_ = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocketDescriptor_ == -1) {
        cout << "Error: no se pudo abrir el socket" << endl;
        exit(1);
    }
}

/*
 * server data structure (struct sockaddr_in serverSocketData_)
 *
 * "inet_addr, que realiza una conversión de las cadenas de 
 *  caracteres de la dirección IP en el formato convencional al formato adecuado para ser utilizado 
 *  en la estructura sockaddr_in"
 * 
*/

void Client::setServerSocketDataStructure_(string serverIpAddress, int serverPortNumber){

    serverSocketData_.sin_family = AF_INET;
    serverSocketData_.sin_port = htons(serverPortNumber);
    serverSocketData_.sin_addr.s_addr = inet_addr(serverIpAddress.c_str());
}


void Client::requestServerConnection_(){}


void Client::setFileDescriptorStructures_(){}


void Client::readServerMessage_(){}


void Client::handleServerErrorMessages_(){}


void Client::sendMessageToServer_(){}


void Client::handleClientMessage_(){}


void Client::startComunication(){}