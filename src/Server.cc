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

        struct timeval tv;
        tv.tv_sec = 0.1; 
        tv.tv_usec = 0; 

        recreateFileDescriptor_();
    
        select(FD_SETSIZE, &this->fileDescriptorSet_, NULL, NULL, &tv);

        // iterate on usersConnected array

        for (User &client: this->usersConnected_) {
            cout << "check for activity on one of connected clients" <<endl;

            if (FD_ISSET(client.getClientSocketDescriptor(), &fileDescriptorSet_)) {
                cout << "client was found, now recv his activity" <<endl;

                if ((recv(client.getClientSocketDescriptor(), &messageBuffer_, BUFFER_SIZE, 0) > 0)) {

                    clientMessageHandler_(client, messageBuffer_);
                    break;
                } 
            }
        }

        // in case that new message arrives at server Socket (sent from a client socket);
        // it enters here the very first time server starts

        if (FD_ISSET(this->serverSocketDescriptor_, &this->fileDescriptorSet_)) {
            handleNewClient_();
        }
    }
    
}



void Server::recreateFileDescriptor_() {

    FD_ZERO(&this->fileDescriptorSet_);                               // clears file descriptor set

    FD_SET(0, &this->fileDescriptorSet_);                             // Watch stdin (fd 0) to see when it has input. 
    FD_SET(this->serverSocketDescriptor_, &this->fileDescriptorSet_); // messages written from client will arrive at server socket

    for (auto user: this->usersConnected_) {
        FD_SET(user.getClientSocketDescriptor(), &this->fileDescriptorSet_);
    }
}



void Server::clientMessageHandler_(User &user, const char* message) {

    std::cmatch RegexMatches;

    cout << "Client: " << user.getClientSocketDescriptor() << " sent: " << message << endl;

    if(strcmp(message, "EXIT\n") == 0){   
        exitClient_(user.getClientSocketDescriptor());


    } else if (strcmp(message, "r\n") == 0){
        if (user.isUserLogged()){ send(user.getClientSocketDescriptor(), "CHECKEO LOGIN", BUFFER_SIZE, 0); }
        send(user.getClientSocketDescriptor(), "Indique su usuario y clave (REGISTRO -u user -p pass)", BUFFER_SIZE, 0);


    } else if(std::regex_search(message, RegexMatches, std::regex("REGISTRO -u (.*) -p (.*)"))){
        //check if client is already registered;
        if( checkIfUserIsWritten(RegexMatches.str(1), RegexMatches.str(2)) == false && user.isUserLogged() == false){

            user.setUserName(RegexMatches.str(1));
            user.setPassword(RegexMatches.str(2));
            user.userNameWasChecked();
            user.passwordWasChecked();

            addUserToFile(user);
            send(user.getClientSocketDescriptor(), "registrado con exito", BUFFER_SIZE, 0);
        } else { send(user.getClientSocketDescriptor(), "ya estas registrado", BUFFER_SIZE, 0); }


    } else if (strcmp(message, "l\n") == 0){
        send(user.getClientSocketDescriptor(), "Indique su usuario: (USUARIO xxx)", BUFFER_SIZE, 0);


    } else if(std::regex_search(message, RegexMatches, std::regex("USUARIO (.*)"))){
        if(user.isUserLogged() == false){
            user.setUserName(RegexMatches.str(1));
            user.userNameWasChecked();
            send(user.getClientSocketDescriptor(), "+Ok.  Usuario  correcto", BUFFER_SIZE, 0);
            send(user.getClientSocketDescriptor(), "Ahora indique su password: (PASSWORD xxx)", BUFFER_SIZE, 0);
        
        } else { send(user.getClientSocketDescriptor(), "???Err.  Usuario  incorrecto", BUFFER_SIZE, 0); }
    

    } else if(std::regex_search(message, RegexMatches, std::regex("PASSWORD (.*)"))){
        if(checkIfUserIsWritten(user.getUserName(), RegexMatches.str(1)) == true && user.isUserNameChecked() == true && user.isUserLogged() == false){

            user.setPassword(RegexMatches.str(1));
            user.passwordWasChecked();
            send(user.getClientSocketDescriptor(), "+Ok. Usuario validado", BUFFER_SIZE, 0);
        
        } else { send(user.getClientSocketDescriptor(), "???ERR. Error en la validaci??n", BUFFER_SIZE, 0); }
        


    } else if(std::regex_search(message, RegexMatches, std::regex("USUARIO (.*) PASSWORD (.*)"))){
        //check if user appear in users.txt
        user.setUserName(RegexMatches.str(1));
        user.userNameWasChecked();
        user.setPassword(RegexMatches.str(1));
        user.passwordWasChecked();
    

    } else if (strcmp(message, "INICIAR-PARTIDA\n") == 0){
        if(user.isUserLogged()){ 
            send(user.getClientSocketDescriptor(), "se va a buscar partida", BUFFER_SIZE, 0);
            searchMatchForClient_(user); 
        }
    } else if (user.isInGame() == true){

        if(std::regex_search(message, RegexMatches, std::regex("VOCAL (.*)"))){

            for(FillMissingLettersGame &x: games_){
                if(x.getGameId() == user.getGameId()){
                    x.checkVocal(RegexMatches.str(1), user.getClientSocketDescriptor());
                }
            }
        
        } else if(std::regex_search(message, RegexMatches, std::regex("CONSONANTE (.*)"))){

            for(FillMissingLettersGame &x: games_){
                if(x.getGameId() == user.getGameId()){
                    x.checkConsonante(RegexMatches.str(1), user.getClientSocketDescriptor());
                }
            }

        } else if(std::regex_search(message, RegexMatches, std::regex("RESOLVER (.*)"))){

            for(FillMissingLettersGame &x: games_){
                if(x.getGameId() == user.getGameId()){
                    int returned = x.resolve(RegexMatches.str(1), user.getClientSocketDescriptor());
                    if(returned == 1){
                        for(User user: usersConnected_){
                            if(user.getClientSocketDescriptor() == x.getPlayer1SocketDescriptor() || user.getClientSocketDescriptor() == x.getPlayer2SocketDescriptor()){
                                user.exitGame();
                                send(user.getClientSocketDescriptor(), "SALE DE LA PARTIDA", BUFFER_SIZE, 0);
                            }
                        }
                    }
                }
            }
            
        }
            
        send(user.getAdversaryId(), message, BUFFER_SIZE, 0);

    }
    
}


void Server::handleNewClient_() {

    int newClientSocketDescriptor = accept(serverSocketDescriptor_, (struct sockaddr *) &clientSocketData_ , &clientSocketDataSize_);

    if(newClientSocketDescriptor == -1) {
        cout << "Error accepting requests" << endl;
        exit(1);
    
    } else if(numberOfClients_ < MAX_CLIENTS) {
        addClientToServer_(newClientSocketDescriptor);
        send(newClientSocketDescriptor, "+0k. Usuario conectado", BUFFER_SIZE, 0);
        send(newClientSocketDescriptor, "Elige si quieres registrarte (r) o loguearte (l)", BUFFER_SIZE, 0);
    
    } else {
        send(newClientSocketDescriptor, "SERVIDOR COMPLETO", BUFFER_SIZE, 0);
    }
}


void Server::addClientToServer_(int newClientSocketDescriptor) {

    User user(newClientSocketDescriptor);
    usersConnected_.push_back(user);
    FD_SET(user.getClientSocketDescriptor(), &this->fileDescriptorSet_);   
    numberOfClients_++;
    cout << "Client was added to clientsConnected array" << endl;
}


void Server::searchMatchForClient_(User &user) {

    bool adversaryFound = false;

    if( usersConnected_.size() > 1 ){

        while(adversaryFound == false){

            for(User &adversary: usersConnected_){

                if( adversary.getClientSocketDescriptor()!=user.getClientSocketDescriptor() && adversary.isInGame()==false && adversary.isUserLogged()==true ){

                    cout << "partida entre " << adversary.getClientSocketDescriptor() << " y " << user.getClientSocketDescriptor() << endl;

                    send(user.getClientSocketDescriptor(), "partida encontrada", BUFFER_SIZE, 0);
                    send(adversary.getClientSocketDescriptor(), "partida encontrada", BUFFER_SIZE, 0);

                    adversary.getInGame();
                    user.getInGame();

                    adversaryFound = true;

                    user.setAdversaryId(adversary.getClientSocketDescriptor());
                    adversary.setAdversaryId(user.getClientSocketDescriptor());

                    user.setGameId(games_.size()+1);
                    adversary.setGameId(games_.size()+1);

                    FillMissingLettersGame game(games_.size()+1, user.getClientSocketDescriptor(), adversary.getClientSocketDescriptor(), this->serverSocketDescriptor_);
                    this->games_.push_back(game);          

                    break;
                }
                
            }
        }
    
    } else { send(newClientSocketDescriptor_, "Estas solo en el servidor", BUFFER_SIZE, 0); }
}


void Server::registerUser(string userName, string password){

    ofstream UsersFile("usuarios.txt"); 
    UsersFile<<userName<<";"<<password<<std::endl;
    UsersFile.close();
}



void Server::exitClient_(int clientSocketDescriptor) {

    sprintf(messageBuffer_, "You are going to exit the server\n");
    send(clientSocketDescriptor, messageBuffer_, BUFFER_SIZE, 0);
    close(clientSocketDescriptor);
    FD_CLR(clientSocketDescriptor, &this->fileDescriptorSet_);
}


void Server::addUserToFile(User user){

    string filename("./db/users.txt");
    ofstream f;
    f.open(filename, std::ios::app);                         //Open the file to add text
    if (f.fail())
        EXIT_FAILURE;

    f << user.getUserName()
      << " ";
    f << user.getPassword();
    f << "\n";

    f.close();
}

bool Server::checkIfUserIsWritten(string userName, string password){

    string aux;         //Here we store the values we read from the file
    size_t len = 0;     //Para empezar al principio de cada linea en el fichero

    string filename("./db/users.txt");

    std::ifstream f(filename);
    if (!f)
        EXIT_FAILURE;

    while (!f.eof()){

        getline(f, aux, ' ');

        if( userName == aux ){

            getline(f, aux, '\n');

            if( password == aux ){
                f.close();
                return true;
            }
        }

        getline(f, aux, '\n');
    }

    f.close();
    return false;
}
