#include "Client.cc"

int main () {
    Client client("127.0.0.1", 2000);
    client.startCommunication();
    return 0;
}