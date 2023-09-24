#include <unistd.h>
#include <string.h>
#include <string>
#include <cctype>

#include "Serializable.h"
#include "Vector2D.h"

#define SERVER_MESSAGE_SIZE sizeof(ServerMessageType) + sizeof(ActionType) + 5 * sizeof(Vector2D) + 2 * sizeof(float) + sizeof(bool) + sizeof(ServerState) + sizeof(int)

class MessageServer: public Serializable
{
public:
    enum ServerMessageType : uint8_t {
        UPDATE_STATE = 0,
        UPDATE_INFO = 1,
        ACTION = 2,
        CHECK_GRID = 3
    };

    enum ServerState : uint8_t {
        WAITING = 0,
        READY = 1,
        PLAYING = 2,
        GAME_OVER = 3,
        SERVER_QUIT = 4,
        EMPTY = 6
    };

    enum ActionType : uint8_t {
        NONE = 0,
        PLACE_RED_TOKEN = 1,
        PLACE_YELLOW_TOKEN = 2
    };

    MessageServer() {};

    MessageServer(int col_) : col(col_) {};

    MessageServer(const ServerState s) : state(s) {};

    void to_bin();

    int from_bin(char* bobj);

public:

    ServerMessageType type;
    ActionType action_;

    int col;

    std::string color;

    ServerState state;
};