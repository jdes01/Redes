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
 * open client socket method; calls the socket functions and return the descriptor to clientSocketDescriptor; 
 *
 * then check for errors
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
 * connect method use the client socket descriptot, server socket data and server socket data size
 * 
*/
void Client::requestServerConnection_(){

    socklen_t serverSocketDataSize = sizeof(this->serverSocketData_);

    int connectionResult = connect(this->clientSocketDescriptor_, (struct sockaddr *) &this->serverSocketData_, serverSocketDataSize);

    if (connectionResult == -1) {
        cout << "Error: imposible contectarse" << endl;
        exit(1);
    }
}


/*
 * start communication method
 *
 * FD_ISSET : macro para manjear fd_set; Mira si el descriptor de socket dado por fd se encuentra en el conjunto especificado por set. 
*/
void Client::startCommunication(){

    while (this->endCommunication_ == false) {

        setFileDescriptorStructures_(); //fd_sets

        select(this->clientSocketDescriptor_ + 1, &this->fileDescriptorSet, NULL, NULL, NULL); // read from the file descriptor stored at fileDescriptorSet
        
        if(FD_ISSET(this->clientSocketDescriptor_, &this->fileDescriptorSet)) {
            readServerMessage_();
        }
        
        else if (FD_ISSET(0, &this->fileDescriptorSet)) {
            sendMessageToServer_();
        }
    }

    closeClient();
}


/*
 * set the file descriptor structure attributes
 *
 * Estos macros se utilizan para manejar el conjunto fd_set que sera usado por la funcion select
 *
 * FD_ZERO This macro clears (removes all file descriptors) from set.
 * 
 * FD_SET añaden un descriptor (0 (escritura por teclado) y clientSocketDescriptor) a fileDescriptorSet
*/
void Client::setFileDescriptorStructures_(){

    FD_ZERO(&this->fileDescriptorSet); // clears the file descriptor set

    FD_SET(0, &this->fileDescriptorSet);                       // add stdin (file descriptor 0) to fileDescriptorSet tp see when it has input.
    FD_SET(this->clientSocketDescriptor_, &this->fileDescriptorSet); // add client socket descriptor to the set because messages written from server will arrive at clientSocket
}


/*
 * read server message method
*/
void Client::readServerMessage_(){

    bzero(this->messageBuffer_, sizeof(this->messageBuffer_)); // clears the message Buffer memory

    recv(this->clientSocketDescriptor_, this->messageBuffer_, sizeof(this->messageBuffer_), 0); //read from clientSocket (what comes from server)
            
    cout << this->messageBuffer_ << endl;
    handleServerErrorMessages_();
}


/*
 * check for the messageBuffer
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

    bzero(this->messageBuffer_, sizeof(this->messageBuffer_));

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

    if(strcmp(messageBuffer_, "EXIT\n") == 0) {
        this->endCommunication_ = true;
    }
}