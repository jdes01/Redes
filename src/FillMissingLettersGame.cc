#include "FillMissingLettersGame.h"

FillMissingLettersGame::FillMissingLettersGame(int id, int player1_SocketDescriptor, int player2_SocketDescriptor, int server_SocketDescriptor){

    this->gameId_ = id;
    this->player1_SocketDescriptor_ = player1_SocketDescriptor;
    this->player2_SocketDescriptor_ = player2_SocketDescriptor;

    this->server_SocketDescriptor_ = server_SocketDescriptor;

    send(player1_SocketDescriptor_, "\n LA PARTIDA VA A COMENZAR", BUFFER_SIZE, 0);
    send(player2_SocketDescriptor_, "\n LA PARTIDA VA A COMENZAR", BUFFER_SIZE, 0);

    send(player1_SocketDescriptor_, "\n LA FRASE ES ESTA:", BUFFER_SIZE, 0);
    send(player2_SocketDescriptor_, "\n LA FRASE ES ESTA", BUFFER_SIZE, 0);

    string sentences[] =
    {
    "a quien madruga, dios le ayuda",
    "no hay mal que por bien no venga",
    "de tal palo, tal astilla",
    "en casa del herrero cuchara de palo",
    "el que no corre, vuela",
    "a lo hecho, pecho",
    "ojo por ojo, diente por diente",
    "a rey muerto, rey puesto",
    "a caballo regalado no le mires el diente",
    "a buenas horas mangas verdes"
    };

    srand(time(NULL));
    int n = rand() % 10;
    this->sentence_ = sentences[n];

    // set secret sentence
    for (int i=0; i<sentence_.length(); i++) {
    if (sentence_[i] == ' '){ gameSentence_+=' '; }
    else if (sentence_[i] == ','){ gameSentence_+=','; }
    else{ gameSentence_+='_'; }
    }

    char* c = const_cast<char*>(sentence_.c_str());
    char* sc = const_cast<char*>(gameSentence_.c_str());

    send(player1_SocketDescriptor_, sc, BUFFER_SIZE, 0);
    send(player2_SocketDescriptor_, sc, BUFFER_SIZE, 0);

    send(player1_SocketDescriptor_, "\n", BUFFER_SIZE, 0);
    send(player2_SocketDescriptor_, "\n", BUFFER_SIZE, 0);

    send(player1_SocketDescriptor_, c, BUFFER_SIZE, 0);
    send(player2_SocketDescriptor_, c, BUFFER_SIZE, 0);
}


void FillMissingLettersGame::startGame(){

    send(player1_SocketDescriptor_, "\n NUEVA PARTIDA", BUFFER_SIZE, 0);
    send(player2_SocketDescriptor_, "\n NUEVA PARTIDA", BUFFER_SIZE, 0);

    while(true){

        select(FD_SETSIZE, &this->gameFileDescriptorSet_, NULL, NULL, NULL);

        if (FD_ISSET(this->player1_SocketDescriptor_, &this->gameFileDescriptorSet_)) {
            if ((recv(this->player1_SocketDescriptor_, &gameMessageBuffer_, BUFFER_SIZE, 0) > 0)){
                
                this->handleMessage(gameMessageBuffer_);
            } 
        }

    }
}


void FillMissingLettersGame::recreateFileDescriptor(){

    FD_ZERO(&this->gameFileDescriptorSet_);

    FD_SET(this->server_SocketDescriptor_, &this->gameFileDescriptorSet_);
    FD_SET(this->player1_SocketDescriptor_, &this->gameFileDescriptorSet_);
}


void FillMissingLettersGame::handleMessage(char * message){

    send(player1_SocketDescriptor_, message, BUFFER_SIZE, 0);
    send(player2_SocketDescriptor_, message, BUFFER_SIZE, 0);
}

void FillMissingLettersGame::checkVocal(string vocal){

    for (int i=0; i<sentence_.length(); i++) {
        if (sentence_[i] == vocal[0]){ gameSentence_[i] = vocal[0]; }
    }

    char* sc = const_cast<char*>(gameSentence_.c_str());

    send(player1_SocketDescriptor_, sc, BUFFER_SIZE, 0);
    send(player2_SocketDescriptor_, sc, BUFFER_SIZE, 0);
}