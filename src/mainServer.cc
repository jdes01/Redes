#include "Server.cc"
#include "User.cc"

int main() {
    Server server(2000);
    server.startServer();
    return 0;
}