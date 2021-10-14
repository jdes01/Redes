#include "Client.h"

/*
 * Client constructor, will be summoned in client main program
*/

Client::Client(string serverIpAddress, int serverPortNumber){

    openClientSocket_();
    fillServerSocketDataStructure_(serverIpAddress, serverPortNumber);
    requestServerConnection_();
    endComunication_ = false;
}

/*
 * open client socket method
*/

void Client::openClientSocket_(){

    clientSocketDescriptor_ = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocketDescriptor_ == -1) {
        cout << "Error: no se pudo abrir el socket" << endl;
        exit(1);
    }
}


void Client::fillServerSocketDataStructure_(string serverIpAddress, int serverPortNumber){}


void Client::requestServerConnection_(){}


void Client::setFileDescriptorStructures_(){}


void Client::readServerMessage_(){}


void Client::handleServerErrorMessages_(){}


void Client::sendMessageToServer_(){}


void Client::handleClientMessage_(){}


void Client::startComunication(){}