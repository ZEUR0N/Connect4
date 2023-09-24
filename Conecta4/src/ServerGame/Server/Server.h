#include <unistd.h>
#include <string>
#include <mutex>
#include <vector>

#include "../../Utils/Socket.h"
#include "../../Utils/MessageClient.h"
#include "../../Utils/MessageServer.h"

#include "../../Utils/Vector2D.h"

#define TICK_RATE 16666 // microseconds

class Server {
public:
    Server(const char * s, const char * p);

    // net messages handler
    void game_thread();
    
    // main game loop
    void run();
private:
    Socket server_socket;
    Socket* player_1, *player_2;

    std::mutex input_mutex;

    MessageServer::ServerState state;
    bool p1_ready, p2_ready;

    MessageClient::InputType input_p1, input_p2;

    int win_width, win_height;


    bool place_t1, place_t2;

    int timer;

    void reset();

    void saveInput(Socket* player_sock, MessageClient::InputType input);
    void handleInput();

    bool addPlayer(Socket* player_sock, int& pl);
    void removePlayer(Socket* player_sock);

    void initPlayer(const int& pl, const MessageClient* msg);

    void stepSimulation();
    void updateInfoClients();

    void sendStateMessage();

    void checkGrid();
};