#include <unistd.h>
#include <iostream>

#include "./Local/Client.h"

void start(char **argv){

    Client g(argv[1], argv[2]);

    g.init(1080, 720);
    g.run();
    g.shutdown();
}

int main(int argc, char *argv[]){
    try {
        if(argc < 3){
            std::cout << "Usage-> ./Server_Connect4.out <ip>(0.0.0.0) <port>(2000)\n";
            return 0;
        }
        start(argv);
    }
    catch(const std::string& e)
    {
        std::cerr << e << '\n';
    }
    catch(const char* e)
    {
        std::cerr << e << '\n';
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch(...)
    {
        std::cerr << "Caught an exception ok unknown type...\n";
    }
    
    return 0;
}