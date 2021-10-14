#include "Server.h"

Server::Server(int serverPort) {}

void Server::openServerSocket_() {}

void Server::fillServerSocketDataStructure_(int serverPortNumber) {}

void Server::assingServerAddressToServerSocket_() {}

void Server::markServerSocketAsPassiveSocket_() {}

void Server::recreateFileDescriptor_() {}

void Server::startServer() {}

void Server::handleNewClient_() {}

void Server::addClientToServer_() {}

void Server::addClientsToServer(vector <int> clientsToAdd) {}

void Server::sendTooManyClientsMessageToNewClient_() {}

void Server::exitClient_(int clientSocketDescriptor) {}

void Server::clientMessageHandler_(int clientSocketDescriptor) {}

void Server::searchMatchForClient_(int clientSocketDescriptor) {}

void Server::sendMessageBufferToAllPlayers_(vector <int> gamePlayers) {}

void Server::createDominoGame_(vector <int> gamePlayers){}

void Server::eraseClientsReadyForGame_(vector <int> gamePlayers) {}

void Server::serverMessageHandler_() {}

void Server::closeServer_() {}