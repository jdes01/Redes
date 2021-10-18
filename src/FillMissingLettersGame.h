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
        int gameId_;
        int player1_SocketDescriptor_;
        int player2_SocketDescriptor_;

        int server_SocketDescriptor_;

        int playerInTurnSocketDescriptor_;

        char gameMessageBuffer_[BUFFER_SIZE];

        fd_set gameFileDescriptorSet_;


        void recreateFileDescriptor();

        void handleMessage(char * message);

        string sentence_;
        string gameSentence_;

    public:
        FillMissingLettersGame(int id, int player1_SocketDescriptor, int player2_SocketDescriptor, int server_SocketDescriptor);
        void startGame();

         int getGameId(){ return this->gameId_; }
        void setGameId(int id){ this->gameId_ = id; }


};
#endif