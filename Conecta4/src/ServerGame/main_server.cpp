#include <iostream>
#include <thread>

#include "./Server/Server.h"

int main(int argc, char **argv) {
    if (argc < 3) {
        std::cout << "Usage-> ./Connect4_server.out <ip>(0.0.0.0) <port>(2000)\n";
        return -1;
    }

    Server server(argv[1], argv[2]);

    std::thread([&server](){
        server.game_thread();
    }).detach();

    server.run();

    return 0;
}