#include "FillMissingLettersGame.h"

FillMissingLettersGame::FillMissingLettersGame(int player1_SocketDescriptor, int player2_SocketDescriptor){

    this->player1_SocketDescriptor_ = player1_SocketDescriptor;
    this->player2_SocketDescriptor_ = player2_SocketDescriptor;
}

void FillMissingLettersGame::startGame(){

    send(player1_SocketDescriptor_, "hola", BUFFER_SIZE, 0);

    send(player2_SocketDescriptor_, "hola", BUFFER_SIZE, 0);
}