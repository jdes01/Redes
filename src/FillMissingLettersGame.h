#ifndef FILL_MISSING_LETTERS_GAME_H
#define FILL_MISSING_LETTERS_GAME_H

#define BUFFER_SIZE 128

#include <sys/socket.h>

class FillMissingLettersGame{

    private:
        int player1_SocketDescriptor_;
        int player2_SocketDescriptor_;

    public:
        FillMissingLettersGame(int player1_SocketDescriptor, int player2_SocketDescriptor);
        void startGame();


};
#endif