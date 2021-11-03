#ifndef FILLMISSINGLETTERSGAME_H
#define FILLMISSINGLETTERSGAME_H

#define BUFFER_SIZE 128


// select() and fd_set
#include <sys/select.h>

// socket() and macros
#include <sys/types.h>
#include <sys/socket.h>

// String
#include <string>
using std::string;

class FillMissingLettersGame{

    private:
        int gameId_;
        int player1_SocketDescriptor_;
        int player2_SocketDescriptor_;

        int playerTurn_;

        int server_SocketDescriptor_;

        int playerInTurnSocketDescriptor_;

        char gameMessageBuffer_[BUFFER_SIZE];

        fd_set gameFileDescriptorSet_;


        void recreateFileDescriptor();

        void handleMessage(char * message);

        string sentence_;
        string gameSentence_;

        int player1_score = 0;
        int player2_score = 0;

    public:
        FillMissingLettersGame(int id, int player1_SocketDescriptor, int player2_SocketDescriptor, int server_SocketDescriptor);
        void startGame();

         int getGameId(){ return this->gameId_; }
        void setGameId(int id){ this->gameId_ = id; }
        
        void checkVocal(string vocal, int playerId);
        void checkConsonante(string consonante, int playerId);

        string getGameSentence(){ return this->gameSentence_; }

        int checkGame();

        int resolve(string solution, int playerId);

        int getPlayer1SocketDescriptor(){ return this->player1_SocketDescriptor_; }
        int getPlayer2SocketDescriptor(){ return this->player2_SocketDescriptor_; }

};
#endif