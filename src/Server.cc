#include "Server.h"

Server::Server(int serverPort) {

    // Open server socket:

    this->serverSocketDescriptor_ = socket (AF_INET, SOCK_STREAM, 0);
    if(this->serverSocketDescriptor_ == -1) {
        cout << "No se pudo abrir el socket del servidor" << endl;
        exit(1);
    }

    // Set server socket data structure

    serverSocketData_.sin_family = AF_INET;
    serverSocketData_.sin_addr.s_addr = INADDR_ANY;
    serverSocketData_.sin_port = htons(serverPort);

    // Assign server address to server socket

    socklen_t serverSocketDataSize = sizeof(serverSocketData_);
    int bindResult = bind(serverSocketDescriptor_, (struct sockaddr *) &serverSocketData_, serverSocketDataSize);
    if (bindResult == -1) {
		cout << "Error with bind operation" << endl;
		exit(1);
	}

    // Mark server socket as pasive socket

    int listenResult = listen(serverSocketDescriptor_, MAX_CLIENTS);
    if(listenResult == -1) {
        cout << "Error with listen operation" << endl;
        exit(1);
    }
}


void Server::startServer() {}



void Server::recreateFileDescriptor_() {}

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