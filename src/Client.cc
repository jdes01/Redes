#include "Client.h"

/*
 * Client constructor, will be summoned in client main program
*/

Client::Client(string serverIpAddress, int serverPortNumber){

    openClientSocket_();
    setServerSocketDataStructure_(serverIpAddress, serverPortNumber);
    requestServerConnection_();
    this->endComunication_ = false;
}

/*
 * open client socket method; calls the socket and check for errors
*/

void Client::openClientSocket_(){

    this->clientSocketDescriptor_ = socket(AF_INET, SOCK_STREAM, 0);

    if(this->clientSocketDescriptor_ == -1) {
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

/*
 * request server connection method
 *
 * connect() method:
 * 
 * (1º argumento, sockfd),  es el descriptor del socket devuelto por la función socket().
 * 
 * (2º argumento, serv_addr), estructura sockaddr que contiene la dirección IP y número de puerto destino.
 * 
 * (3º  argumento,  serv_addrlen),  debe  ser  inicializado  al  tamaño  de  struct  sockaddr. sizeof(struct sockaddr).
 * 
*/

void Client::requestServerConnection_(){

    socklen_t serverSocketDataSize;
    serverSocketDataSize = sizeof(serverSocketData_);

    int connectionResult = connect(this->clientSocketDescriptor_, (struct sockaddr *) &this->serverSocketData_, serverSocketDataSize);

    if (connectionResult == -1) {
        cout << "Error: imposible contectarse" << endl;
        exit(1);
    }
}

/*
 * set the file descriptor structure attributes
 *
 * Estos macros se utilizan para manejar el conjunto fd_set que sera usado por la funcion select
 *
 * FD_ZERO inicializa un conjunto fd_set especificado por 'readerFileDescriptor_'
 * 
 * FD_SET añaden un descriptor (0 y clientSocketDescriptor) a readerFileDescriptor
*/

void Client::setFileDescriptorStructures_(){

    FD_ZERO(&this->readerFileDescriptor_);
    FD_SET(0, &this->readerFileDescriptor_);
    FD_SET(clientSocketDescriptor_, &this->readerFileDescriptor_);
}


void Client::readServerMessage_(){}


void Client::handleServerErrorMessages_(){}


void Client::sendMessageToServer_(){}


void Client::handleClientMessage_(){}


void Client::startComunication(){}