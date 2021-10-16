#include "Server.h"

Server::Server(int serverPort) {

    // Open server socket:

    this->serverSocketDescriptor_ = socket (AF_INET, SOCK_STREAM, 0);
    if(this->serverSocketDescriptor_ == -1) {
        cout << "No se pudo abrir el socket del servidor" << endl;
        exit(1);
    }

    // Set server socket data structure

    this->serverSocketData_.sin_family = AF_INET;
    this->serverSocketData_.sin_addr.s_addr = INADDR_ANY;
    this->serverSocketData_.sin_port = htons(serverPort);

    // Assign server address to server socket

    socklen_t serverSocketDataSize = sizeof(this->serverSocketData_);

    int bindResult = bind(serverSocketDescriptor_, (struct sockaddr *) &this->serverSocketData_, serverSocketDataSize);
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


void Server::startServer() {

    while (true) {

        cout << "1" <<endl;

        recreateFileDescriptor_();

        cout << "2" <<endl;
    
        select(FD_SETSIZE, &this->fileDescriptorSet_, NULL, NULL, NULL);

        cout << "3" <<endl;

        // iterate on clientsConnected array

        for (auto clientSocketDescriptor = this->clientsConnected_.begin(); clientSocketDescriptor != this->clientsConnected_.end(); ++clientSocketDescriptor) {

            cout << "detecct activity on one of connected clients" <<endl;

            if (FD_ISSET(*clientSocketDescriptor, &fileDescriptorSet_)) {

                cout << "client was found, now recv his activity" <<endl;

                if ((recv(*clientSocketDescriptor, &messageBuffer_, BUFFER_SIZE, 0) > 0)) {

                    clientMessageHandler_(*clientSocketDescriptor, messageBuffer_);
                    break;
                } 
            }
        }

        // in case that new message arrives at server Socket (sent from a client socket);
        // it enters here the very first time server starts

        if (FD_ISSET(this->serverSocketDescriptor_, &this->fileDescriptorSet_)) {
            cout << "detect server socket activity" <<endl;
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
        cout << "x" <<endl;
        FD_SET(*clientSocketDescriptor, &this->fileDescriptorSet_);
    }
}


void Server::clientMessageHandler_(int clientSocketDescriptor, const char* message) {

    std::cmatch RegexMatches;

    cout << "Client: " << clientSocketDescriptor << " sent: " << message << endl;

    if(strcmp(message, "EXIT\n") == 0){   

        exitClient_(clientSocketDescriptor);
    
    } else if (strcmp(message, "INICIAR-PARTIDA\n") == 0){

        cout << "!!!!" << endl;

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

           if ((recv(clientSocketDescriptor, &messageBuffer_, BUFFER_SIZE, 0) > 0)) {

                if(std::regex_search(messageBuffer_, RegexMatches, std::regex("PASSWORD (.*)"))){

                    logInClient(clientSocketDescriptor, RegexMatches.str(1)); // TODO: need to login
                }

            } else { send(clientSocketDescriptor, "-ERR. Formato para la password incorrecto", BUFFER_SIZE, 0); }
        
        } else { send(clientSocketDescriptor, "-ERR. Ya estas Logueado", BUFFER_SIZE, 0); }
    }
}


void Server::handleNewClient_() {

    cout << "Handle new client" << endl;

    newClientSocketDescriptor_ = accept(serverSocketDescriptor_, (struct sockaddr *) &clientSocketData_ , &clientSocketDataSize_);

    if(newClientSocketDescriptor_ == -1) {
        cout << "Error accepting requests" << endl;
        exit(1);
    
    } else if(numberOfClients_ < MAX_CLIENTS) {

        if( registerOrLoginProcess(newClientSocketDescriptor_) == 0 ){
            send(newClientSocketDescriptor_, "jugador expulsado", BUFFER_SIZE, 0);
            exitClient_(newClientSocketDescriptor_);
            
        } else{
            cout << "it goes to add client to server function" <<endl;
            addClientToServer_();
        }
    
    } else {
        sendTooManyClientsMessageToNewClient_();
    }
}


int Server::registerOrLoginProcess(int newClientSocketDescriptor_){

    std::cmatch RegexMatches;

    send(newClientSocketDescriptor_, "Elige si quieres registrarte (r) o loguearte (l)", BUFFER_SIZE, 0);
    recv(newClientSocketDescriptor_, &messageBuffer_, BUFFER_SIZE, 0);

    if (strcmp(messageBuffer_, "l\n") == 0){

        send(newClientSocketDescriptor_, "Indique su usuario: (USUARIO xxx)", BUFFER_SIZE, 0);
        recv(newClientSocketDescriptor_, &messageBuffer_, BUFFER_SIZE, 0);

        if( std::regex_search(messageBuffer_, RegexMatches, std::regex("USUARIO (.*)")) ){

            send(newClientSocketDescriptor_, "Ahora indique su password: (PASSWORD xxx)", BUFFER_SIZE, 0);
            recv(newClientSocketDescriptor_, &messageBuffer_, BUFFER_SIZE, 0);

            if(std::regex_search(messageBuffer_, RegexMatches, std::regex("PASSWORD (.*)"))){

                    //logInClient(newClientSocketDescriptor_, RegexMatches.str(1)); // TODO: need to login
                    return 1;
            }

        }
    }
    if (strcmp(messageBuffer_, "r\n") == 0){

        send(newClientSocketDescriptor_, "jugador registrado", BUFFER_SIZE, 0);
        return 1;
    }

    return 0;
}


void Server::addClientToServer_() {

    clientsConnected_.push_back(newClientSocketDescriptor_);
    FD_SET(newClientSocketDescriptor_, &this->fileDescriptorSet_);   
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


void Server::exitClient_(int clientSocketDescriptor) {

    sprintf(messageBuffer_, "You are going to exit the server\n");
    send(clientSocketDescriptor, messageBuffer_, BUFFER_SIZE, 0);
    close(clientSocketDescriptor);
    FD_CLR(clientSocketDescriptor, &this->fileDescriptorSet_);
}


void Server::sendMessageBufferToAllPlayers_(vector <int> gamePlayers) {}

void Server::createDominoGame_(vector <int> gamePlayers){}

void Server::eraseClientsReadyForGame_(vector <int> gamePlayers) {}

void Server::serverMessageHandler_() {}

void Server::closeServer_() {}