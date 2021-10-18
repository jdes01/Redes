#ifndef USER_H
#define USER_H

// String
#include <string>
using std::string;

#include "FillMissingLettersGame.h"

class User {

    private:
        int clientSocketDescriptor_;
        string userName_;
        string password_;

        bool isUserNameChecked_;
        bool isPasswordChecked_;

        bool isInGame_;

        int adversaryId_;

        FillMissingLettersGame game_;

    public:
        User(int clientSocketDescriptor);
        int getClientSocketDescriptor(){ return this->clientSocketDescriptor_; }
        string getUserName(){ return this->userName_; }
        string getPassword(){ return this->password_; }
        bool isInGame(){ return isInGame_; }

        void setClientSocketDescriptor(int socketDescriptor){ this->clientSocketDescriptor_ = socketDescriptor; }
        void setUserName(string userName){ this->userName_ = userName; }
        void setPassword(string password){ this->password_ = password; }

        void userNameWasChecked(){ this->isUserNameChecked_ = true; }
        void passwordWasChecked(){ this->isPasswordChecked_ = true; }
        
        void getInGame(){ this->isInGame_ = true; }
        void exitGame() { this->isInGame_ = false; }

        bool isUserLogged();

        bool isUserNameChecked(){ return isUserNameChecked_; }
        bool isPasswordChecked(){ return isPasswordChecked_; }

         int getAdversaryId(){ return adversaryId_; }
        void setAdversaryId(int id){ this->adversaryId_ = id; }

        FillMissingLettersGame getGame(){ return this->game_; }
        void setGame(FillMissingLettersGame game){ this->game_ = game; }

};
#endif