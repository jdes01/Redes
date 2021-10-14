#include "Client.h"

/*
 * Client constructor, will be summoned in client main program
*/

Client::Client(string serverIpAddress, int serverPortNumber){

    openClientSocket_();
    setServerSocketDataStructure_(serverIpAddress, serverPortNumber);
    requestServerConnection_();
    this->endCommunication_ = false;
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

/*
 * read server message method
 * 
*/

void Client::readServerMessage_(){

    recv(this->clientSocketDescriptor_, this->messageBuffer_, sizeof(this->messageBuffer_), 0);
            
    cout << this->messageBuffer_ << endl;
    handleServerErrorMessages_();
}

/*
 * check for the messageBuffer
 * 
*/

void Client::handleServerErrorMessages_(){

    //string comparison: return 0 if first and second argument are equal

    if(strcmp(this->messageBuffer_, "Demasiados clientes conectados\n") == 0) {
        this->endCommunication_ = true;
    }
}

/*
 * send message to server method
 *
 * char *fgets(char *str, int n, FILE *stream) reads a line from the specified stream 
 * and stores it into the string pointed to by str. 
 * It stops when either (n-1) characters are read, the newline character is read, 
 * or the end-of-file is reached, whichever comes first.
*/ 

void Client::sendMessageToServer_(){

    // writes on messageBuffer until messafeBuffer ends or newline is written (stdin reads from terminal)

    fgets(this->messageBuffer_, sizeof(this->messageBuffer_), stdin);

    handleClientMessage_();
    send(this->clientSocketDescriptor_, this->messageBuffer_, sizeof(this->messageBuffer_), 0);
}

/*
 * handle client message method
 *
 * check the client message to end up communication in case of exit message
*/

void Client::handleClientMessage_(){

    // string comparison

    if(strcmp(messageBuffer_, "SALIR\n") == 0) {
        this->endCommunication_ = true;
    }
}

/*
 * start communication method
 *
 * FD_ISSET : macro para manjear fd_set; Mira si el descriptor de socket dado por fd se encuentra en el conjunto especificado por set. 
*/

void Client::startCommunication(){

    while (this->endCommunication_ == false) {

        setFileDescriptorStructures_();

        select(this->clientSocketDescriptor_ + 1, &this->readerFileDescriptor_, NULL, NULL, NULL);
        
        if(FD_ISSET(this->clientSocketDescriptor_, &this->readerFileDescriptor_)) {
            readServerMessage_();
        }
        
        else if (FD_ISSET(0, &this->readerFileDescriptor_)) {
            sendMessageToServer_();
        }
    }

    closeClient();
}