#include "FillMissingLettersGame.h"

FillMissingLettersGame::FillMissingLettersGame(int id, int player1_SocketDescriptor, int player2_SocketDescriptor, int server_SocketDescriptor){

    this->gameId_ = id;
    this->player1_SocketDescriptor_ = player1_SocketDescriptor;
    this->player2_SocketDescriptor_ = player2_SocketDescriptor;

    playerTurn_ = this->player1_SocketDescriptor_;

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

void FillMissingLettersGame::checkVocal(string vocal, int playerId){

    if(playerId == playerTurn_){

    if( (playerId==this->player1_SocketDescriptor_ && player1_score>=50) || (playerId==this->player2_SocketDescriptor_ && player2_score>=50)) {

        if( vocal == "a" || vocal == "e" || vocal == "i" || vocal == "o" || vocal == "u"){

            int found = 0;

            for (int i=0; i<sentence_.length(); i++) {

                if (sentence_[i]==vocal[0] && gameSentence_[i]=='_'){ 
                    found = 1;
                    gameSentence_[i] = vocal[0];
                }
            }
            if (found == 1){ 
                if(playerId == this->player1_SocketDescriptor_){ this->player1_score-=50; }
                else{ this->player2_score-=50; } 
            
            } else { 

                send(player1_SocketDescriptor_, "CAMBIO DE TURNO", BUFFER_SIZE, 0);
                send(player2_SocketDescriptor_, "CAMBIO DE TURNO", BUFFER_SIZE, 0);

                if(playerId == this->player1_SocketDescriptor_){ playerTurn_ = player2_SocketDescriptor_; }
                else{ playerTurn_ = player2_SocketDescriptor_; }
            }

            char* sc = const_cast<char*>(gameSentence_.c_str());

            send(player1_SocketDescriptor_, sc, BUFFER_SIZE, 0);
            send(player2_SocketDescriptor_, sc, BUFFER_SIZE, 0);

            string s1 = std::to_string(player1_score);
            char const * p1s = s1.c_str();
            send(player1_SocketDescriptor_, p1s, BUFFER_SIZE, 0);

            string s2 = std::to_string(player2_score);
            char const * p2s = s2.c_str();
            send(player2_SocketDescriptor_, p2s, BUFFER_SIZE, 0);
        }

        if(checkGame() == 1){
            send(player1_SocketDescriptor_, "PARTIDA TERMINADA", BUFFER_SIZE, 0);
            send(player2_SocketDescriptor_, "PARTIDA TERMINADA", BUFFER_SIZE, 0);
        
        }
    
    }
    }
}

void FillMissingLettersGame::checkConsonante(string consonante, int playerId){

    if(playerId == playerTurn_){

    if( consonante != "a" && consonante != "e" && consonante != "i" && consonante != "o" && consonante != "u"){

        int found = 0;

        for (int i=0; i<sentence_.length(); i++) {
            if (sentence_[i]==consonante[0] && gameSentence_[i]=='_'){ 
                found = 1;
                gameSentence_[i] = consonante[0];
                if(playerId == this->player1_SocketDescriptor_){ this->player1_score+=50; }
                else{ this->player2_score+=50; } 
            }
        }

        if(found == 0) { 

            send(player1_SocketDescriptor_, "CAMBIO DE TURNO", BUFFER_SIZE, 0);
            send(player2_SocketDescriptor_, "CAMBIO DE TURNO", BUFFER_SIZE, 0);

            if(playerId == this->player1_SocketDescriptor_){ playerTurn_ = player2_SocketDescriptor_; }
            else{ playerTurn_ = player2_SocketDescriptor_; }
            }

        char* sc = const_cast<char*>(gameSentence_.c_str());

        send(player1_SocketDescriptor_, sc, BUFFER_SIZE, 0);
        send(player2_SocketDescriptor_, sc, BUFFER_SIZE, 0);

        string s1 = std::to_string(player1_score);
        char const * p1s = s1.c_str();
        send(player1_SocketDescriptor_, p1s, BUFFER_SIZE, 0);

        string s2 = std::to_string(player2_score);
        char const * p2s = s2.c_str();
        send(player2_SocketDescriptor_, p2s, BUFFER_SIZE, 0);
    }

    if(checkGame() == 1){
        send(player1_SocketDescriptor_, "PARTIDA TERMINADA", BUFFER_SIZE, 0);
        send(player2_SocketDescriptor_, "PARTIDA TERMINADA", BUFFER_SIZE, 0);
    }

    }
}
/*
 * @return 0 if not finished and 1 if finished
 */
int FillMissingLettersGame::checkGame(){

    for (int i=0; i<sentence_.length(); i++) {
            if (gameSentence_[i] == '_'){ return 0; }
    }

    return 1;
}

/*
 * @return 0 if not finished and 1 if finished
 */
int FillMissingLettersGame::resolve(string solution, int playerId){

    if(playerId == playerTurn_){
        if(solution == this->sentence_){

            if(playerId == this->player1_SocketDescriptor_){ 

                send(player1_SocketDescriptor_, "GANASTE", BUFFER_SIZE, 0);
                send(player2_SocketDescriptor_, "ESPABILA", BUFFER_SIZE, 0);
                return 1;

            } else{ 

                send(player2_SocketDescriptor_, "GANASTE", BUFFER_SIZE, 0);
                send(player1_SocketDescriptor_, "ESPABILA", BUFFER_SIZE, 0);
                return 1;
            }
        }
    }

    return 0; 
}