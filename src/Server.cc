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

    // atm it lands here
}


void Server::startServer() {

    while (true) {

        //struct timespec time;
        //time.tv_sec=10;

        // updates file descriptor set; It will contain file descriptors for every client socket from clientsConnected clients array

        recreateFileDescriptor_();
    
        //pselect(FD_SETSIZE, &this->fileDescriptorSet_, NULL, NULL, &time, NULL);
        select(FD_SETSIZE, &this->fileDescriptorSet_, NULL, NULL, NULL);

        // iterate on clientsConnected array

        for (auto clientSocketDescriptor = this->clientsConnected_.begin(); clientSocketDescriptor != this->clientsConnected_.end(); ++clientSocketDescriptor) {

            // in case they wrote and recv reads something, clientMessageHandler will handle it

            cout << "there are clients added in the clientsConnected array" <<endl;

            if (FD_ISSET(*clientSocketDescriptor, &fileDescriptorSet_)) {

                cout << "waits for a client activity" <<endl;

                if ((recv(*clientSocketDescriptor, &messageBuffer_, BUFFER_SIZE, 0) > 0)) {

                    clientMessageHandler_(*clientSocketDescriptor, messageBuffer_);
                    break;
                } 
            }
        }

        // in case that new message arrives at server Socket (sent from a client socket);
        // it enters here the very first time server starts

        if (FD_ISSET(this->serverSocketDescriptor_, &this->fileDescriptorSet_)) {
            cout << "checks for server socker activity" <<endl;
            handleNewClient_();
            cout << "comes out of handleNewClient function" <<endl;
        }

        

        /*
            if(FD_ISSET(0, &readerFileDescriptor_)) {
                fgets(messageBuffer_, BUFFER_SIZE, stdin);
                serverMessageHandler_();
            }
        */

    }
    
}



void Server::recreateFileDescriptor_() {

    FD_ZERO(&this->fileDescriptorSet_); // clears file descriptor set

    FD_SET(0, &this->fileDescriptorSet_);                       // Watch stdin (fd 0) to see when it has input. 
    FD_SET(this->serverSocketDescriptor_, &this->fileDescriptorSet_); // messages written from client will arrive at server socket

    for (auto clientSocketDescriptor = this->clientsConnected_.begin(); clientSocketDescriptor != this->clientsConnected_.end(); ++clientSocketDescriptor) {
        FD_SET(*clientSocketDescriptor, &this->fileDescriptorSet_);
    }
}


void Server::clientMessageHandler_(int clientSocketDescriptor, const char* message) {

    std::cmatch RegexMatches;

    cout << "Client: " << clientSocketDescriptor << " sent: " << message << endl;

    if(strcmp(message, "EXIT") == 0){   

        exitClient_(clientSocketDescriptor);
    

    } else if (strcmp(message, "INICIAR-PARTIDA") == 0){

        if(isClientLogged(clientSocketDescriptor)) {

            searchMatchForClient_(clientSocketDescriptor);
        
        } else { send(clientSocketDescriptor, "-ERR. Te tienes que Loggear Primero", BUFFER_SIZE, 0); }


    } else if(std::regex_search(message, RegexMatches, std::regex("REGISTRO -u (.*) -p (.*)"))){

        if(isClientLogged(clientSocketDescriptor) == false){

            registerUser(RegexMatches.str(1), RegexMatches.str(2));
        
        } else{ send(clientSocketDescriptor, "-ERR. Ya estas Logueado", BUFFER_SIZE, 0); }


    }  else if(std::regex_search(message, RegexMatches, std::regex("USUARIO (.*)"))){

        if(isClientLogged(clientSocketDescriptor) == false){

            send(clientSocketDescriptor, "Ok, Esperando Contraseña", BUFFER_SIZE, 0);

            /*
            this->threads_.push_back(std::async(std::launch::async, [this, clientSocketDescriptor, RegexMatches]{
                return logInClient(clientSocketDescriptor, RegexMatches.str(1));
            }));
            */

           if ((recv(clientSocketDescriptor, &messageBuffer_, BUFFER_SIZE, 0) > 0)) {

                if(std::regex_search(messageBuffer_, RegexMatches, std::regex("PASSWORD (.*)"))){

                    logInClient(clientSocketDescriptor, RegexMatches.str(1)); // TODO: need to login
                }

            } else { send(clientSocketDescriptor, "-ERR. Formato para la password incorrecto", BUFFER_SIZE, 0); }
        
        } else { send(clientSocketDescriptor, "-ERR. Ya estas Logueado", BUFFER_SIZE, 0); }
    }
}


void Server::handleNewClient_() {

    cout << "waits for new client" << endl;

    newClientSocketDescriptor_ = accept(serverSocketDescriptor_, (struct sockaddr *) &clientSocketData_ , &clientSocketDataSize_);

    if(newClientSocketDescriptor_ == -1) {
        cout << "Error accepting requests" << endl;
        exit(1);
    }

    else if(numberOfClients_ < MAX_CLIENTS) {
        cout << "it goes to add client to server function" <<endl;
        addClientToServer_();
    }
    
    else {
        sendTooManyClientsMessageToNewClient_();
    }
}


void Server::addClientToServer_() {

    clientsConnected_.push_back(newClientSocketDescriptor_);
    numberOfClients_++;
    sprintf(messageBuffer_, "Welcome To Server");
    send(newClientSocketDescriptor_, messageBuffer_, BUFFER_SIZE, 0);
    cout << "Client was added to clientsConnected array" << endl;
}


bool Server::isClientLogged(int clientSocketDescriptor){

    for(auto loggedClient: this->clientsConnected_){

        if(loggedClient == clientSocketDescriptor){ return true; }
    }
    return false;
}


int Server::logInClient(int clientSocketDescriptor, string userName){

    return 0;
}


void Server::searchMatchForClient_(int clientSocketDescriptor) {


}


void Server::registerUser(string userName, string password){

    ofstream UsersFile("usuarios.txt"); 

    UsersFile<<userName<<";"<<password<<std::endl;

    UsersFile.close();
}




void Server::addClientsToServer(vector <int> clientsToAdd) {}

void Server::sendTooManyClientsMessageToNewClient_() {}

void Server::exitClient_(int clientSocketDescriptor) {}

void Server::sendMessageBufferToAllPlayers_(vector <int> gamePlayers) {}

void Server::createDominoGame_(vector <int> gamePlayers){}

void Server::eraseClientsReadyForGame_(vector <int> gamePlayers) {}

void Server::serverMessageHandler_() {}

void Server::closeServer_() {}