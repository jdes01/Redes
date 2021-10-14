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


void Client::openClientSocket_(){}


void Client::fillServerSocketDataStructure_(string serverIpAddress, int serverPortNumber){}


void Client::requestServerConnection_(){}


void Client::setFileDescriptorStructures_(){}


void Client::readServerMessage_(){}


void Client::handleServerErrorMessages_(){}


void Client::sendMessageToServer_(){}


void Client::handleClientMessage_(){}


void Client::startComunication(){}