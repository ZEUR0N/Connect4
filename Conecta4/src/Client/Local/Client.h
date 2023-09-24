#ifndef CLIENT_H_
#define CLIENT_H_

#include <unistd.h>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <thread>
#include <chrono>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

#include "../../Utils/Socket.h"

#include "../../Utils/MessageClient.h"
#include "../../Utils/MessageServer.h"

#include "../../Utils/Vector2D.h"

#define GRID_WIDTH_SIZE 640
#define GRID_HEIGHT_SIZE 550

class GameObject;
class SDL_Renderer;
class Grid;
class Token;

struct InitData
{
    Vector2D dim;
    float rot;
};

enum GridState{
    NONE = 0,
    RED = 1,
    YELLOW = 2
};

class Client
{
public:
    Client(const char *s, const char *p);
    virtual ~Client();

    void init(int w, int h);
    void run();
    void shutdown();

    std::vector<GameObject *> *getGOsReference()
    {
        return &objs_;
    };

    void sendGameMessage(MessageClient::InputType input);
    void sendMatchMessage(MessageClient::ClientMessageType msg, InitData *data = nullptr);

private:
    Socket client_socket;

    std::vector<GameObject *> objs_;
    std::vector<GameObject *> gObjsToAdd_;

    std::string color;

    GridState gridState;

    Grid *player_;

    MessageServer::ServerState currentState, nextState;

    void net_message_thread();

    void checkState();
    void changeState(MessageServer::ServerState state);
    void loadScreen(const std::string &textFile, const std::string &fontFile, const std::string &text,
                    const Vector2D &textPos, const Vector2D &textDim, const SDL_Color &color, const int &size);
    void playLoad();

    void refresh();

    void placeToken(std::string color, int col);

    void clearGameObjects();
};
#endif