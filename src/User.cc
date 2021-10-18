#include "User.h"

User::User(int clientSocketDescriptor) {

    this->clientSocketDescriptor_ = clientSocketDescriptor;
    this->isUserNameChecked_ = false;
    this->isPasswordChecked_ = false;
    this->isInGame_ = false;
}

bool User::isUserLogged(){ 
    if(this->isUserNameChecked_ == true && this->isPasswordChecked_ == true){ return true; }
    return false;
}