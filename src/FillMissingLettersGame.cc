#include "FillMissingLettersGame.h"

FillMissingLettersGame::FillMissingLettersGame(int player1_SocketDescriptor, int player2_SocketDescriptor, int server_SocketDescriptor){

    this->player1_SocketDescriptor_ = player1_SocketDescriptor;
    this->player2_SocketDescriptor_ = player2_SocketDescriptor;

    this->server_SocketDescriptor_ = server_SocketDescriptor;
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