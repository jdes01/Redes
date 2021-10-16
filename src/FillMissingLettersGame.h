#ifndef FILL_MISSING_LETTERS_GAME_H
#define FILL_MISSING_LETTERS_GAME_H

#define BUFFER_SIZE 128


// select() and fd_set
#include <sys/select.h>

// socket() and macros
#include <sys/types.h>
#include <sys/socket.h>

class FillMissingLettersGame{

    private:
        int player1_SocketDescriptor_;
        int player2_SocketDescriptor_;

        int server_SocketDescriptor_;

        int playerInTurnSocketDescriptor_;

        char gameMessageBuffer_[BUFFER_SIZE];

        fd_set gameFileDescriptorSet_;


        void recreateFileDescriptor();

        void handleMessage(char * message);

    public:
        FillMissingLettersGame(int player1_SocketDescriptor, int player2_SocketDescriptor, int server_SocketDescriptor);
        void startGame();


};
#endif