#include "Server.h"
#include <time.h>
#include <iostream>

#include <SDL2/SDL.h>

Server::Server(const char *s, const char *p) : server_socket(s, p)
{
    srand((unsigned)time(NULL));

    server_socket.bind();

    player_1 = player_2 = nullptr;

    state = MessageServer::ServerState::WAITING;

    reset();
};

void Server::reset()
{
    input_p1 = input_p2 = MessageClient::InputType::NONE;

    place_t1 = false;

    p1_ready = false;
    p2_ready = false;
}

void Server::game_thread()
{
    while (true)
    {
        MessageClient client_recv_msg;
        Socket *client_player_sock = new Socket(server_socket);
        server_socket.recv(client_recv_msg, client_player_sock);

        switch (client_recv_msg.type)
        {
        case MessageClient::ClientMessageType::REGISTER:
        { // - REGISTER: register player on server data base
            int pl;
            if (addPlayer(client_player_sock, pl))
                initPlayer(pl, &client_recv_msg);

            if (player_1 != nullptr && player_2 != nullptr){
                state = MessageServer::ServerState::READY;
                printf("Tamos Ready");
            }
            else{
                state = MessageServer::ServerState::WAITING;
            }

            sendStateMessage();
        }
        break;
        case MessageClient::ClientMessageType::QUIT:
        { // - QUIT: remove player from server data base
            removePlayer(client_player_sock);

            if (player_1 == nullptr || player_2 == nullptr)
            {
                state = MessageServer::ServerState::WAITING;
                sendStateMessage();
            }
        }
        break;
        case MessageClient::ClientMessageType::HANDLE_INPUT:
        { // - HANDLE_INPUT: handles input sent by client for next simulation step

            input_mutex.lock(); // mutex lock so we don't modify values during run loop (handleInput())
            saveInput(client_player_sock, client_recv_msg.input);
            input_mutex.unlock();
        }
        break;
        }
    }
}

void Server::run()
{
    while (true)
    {
        input_mutex.lock(); // mutex lock so we are save from alterations
        handleInput();
        input_mutex.unlock();

        if (state == MessageServer::ServerState::READY && p1_ready && p2_ready)
        {
            state = MessageServer::ServerState::PLAYING;
            printf("Empezamos\n");
            p1_ready = p2_ready = false;
            timer = 0;
            sendStateMessage();
        }

        if (state == MessageServer::ServerState::PLAYING)
        {
            stepSimulation();
            usleep(TICK_RATE);
        }

        Uint32 startTime = SDL_GetTicks();
        if (state == MessageServer::ServerState::GAME_OVER)
        {
            Uint32 frameTime = SDL_GetTicks() - startTime;
            if(timer <= 3500)
                timer += frameTime;
            else 
            {
                state = MessageServer::ServerState::READY;
                sendStateMessage();
            }
        }
    };
}

bool Server::addPlayer(Socket *player_sock, int &pl)
{
    if ((player_1 && *player_sock == *player_1) || (player_2 && *player_sock == *player_2))
    {
        printf("Player already registered.\n");
        return false;
    }

    if (player_1 != nullptr && player_2 != nullptr)
    {
        printf("Unable to register player, match already full.\n");
        return false;
    }

    if (player_1 == nullptr)
    {
        player_1 = player_sock;
        pl = 0;
        printf("Registered. Player One Ready.\n");
    }
    else
    {
        player_2 = player_sock;
        pl = 1;
        printf("Registered. Player Two Ready.\n");
    }
    return true;
}

void Server::removePlayer(Socket *player_sock)
{
    // I dont know if i should remove too, for now we just nullptr the reference
    if (player_1 != nullptr && *player_sock == *player_1)
    {
        printf("Player One Quit\n");
        player_1 = nullptr;
    }
    else if (player_2 != nullptr && *player_sock == *player_2)
    {
        printf("Player Two Quit\n");
        player_2 = nullptr;
    }
    else
        printf("Player was not registered previously.\n");
}

void Server::initPlayer(const int &pl, const MessageClient *msg)
{
    win_width = msg->win_width;
    win_height = msg->win_height;
}

void Server::saveInput(Socket *player_sock, MessageClient::InputType input)
{
    if (*player_sock == *player_1)
        input_p1 = input;
    else if (*player_sock == *player_2)
        input_p2 = input;
    else
        printf("Trying to handle input from a non registered client.\n");
}

void Server::handleInput()
{
    // handle input player 1
    if(input_p1 != MessageClient::InputType::NONE){
        MessageServer msg(state);
        msg.type = MessageServer::ServerMessageType::ACTION;
        msg.action_ = MessageServer::ActionType::PLACE_RED_TOKEN;
        switch (input_p1)
        {
        case MessageClient::InputType::ONE:
                msg.col = 0;
                if (!place_t1)
            {
                place_t1 = true;
                printf("Place RED.\n");
            }
        server_socket.send(msg, *player_1);
        server_socket.send(msg, *player_2);
        break;
        case MessageClient::InputType::TWO:
                msg.col = 1;
                if (!place_t1)
            {
                place_t1 = true;
                printf("Place RED.\n");
            }
        server_socket.send(msg, *player_1);
        server_socket.send(msg, *player_2);
        break;
        case MessageClient::InputType::THREE:
                msg.col = 2;
                if (!place_t1)
            {
                place_t1 = true;
                printf("Place RED.\n");
            }
        server_socket.send(msg, *player_1);
        server_socket.send(msg, *player_2);
        break;
        case MessageClient::InputType::FOUR:
                msg.col = 3;
                if (!place_t1)
            {
                place_t1 = true;
                printf("Place RED.\n");
            }
        server_socket.send(msg, *player_1);
        server_socket.send(msg, *player_2);
        break;
        case MessageClient::InputType::FIVE:
                msg.col = 4;
                if (!place_t1)
            {
                place_t1 = true;
                printf("Place RED.\n");
            }
        server_socket.send(msg, *player_1);
        server_socket.send(msg, *player_2);
        break;
        case MessageClient::InputType::SIX:
                msg.col = 5;
                if (!place_t1)
            {
                place_t1 = true;
                printf("Place RED.\n");
            }
        server_socket.send(msg, *player_1);
        server_socket.send(msg, *player_2);
        break;
        case MessageClient::InputType::SEVEN:
                msg.col = 6;
                if (!place_t1)
            {
                place_t1 = true;
                printf("Place RED.\n");
            }
        server_socket.send(msg, *player_1);
        server_socket.send(msg, *player_2);
        break;
        case MessageClient::InputType::PLAY:
            p1_ready = true;
        break;
        default:
        break;
        }
        
    }

    // handle input player 2
    if(input_p2 != MessageClient::InputType::NONE){
        MessageServer msg(state);
        msg.type = MessageServer::ServerMessageType::ACTION;
        msg.action_ = MessageServer::ActionType::PLACE_YELLOW_TOKEN;
        switch (input_p2)
        {
        case MessageClient::InputType::ONE:
                msg.col = 0;
                if (!place_t2)
            {
                place_t2 = true;
                printf("Place YELLOW.\n");
            }
        server_socket.send(msg, *player_1);
        server_socket.send(msg, *player_2);
        break;
        case MessageClient::InputType::TWO:
                msg.col = 1;
                if (!place_t2)
            {
                place_t2 = true;
                printf("Place YELLOW.\n");
            }
        server_socket.send(msg, *player_1);
        server_socket.send(msg, *player_2);
        break;
        case MessageClient::InputType::THREE:
                msg.col = 2;
                if (!place_t2)
            {
                place_t2 = true;
                printf("Place YELLOW.\n");
            }
        server_socket.send(msg, *player_1);
        server_socket.send(msg, *player_2);
        break;
        case MessageClient::InputType::FOUR:
                msg.col = 3;
                if (!place_t2)
            {
                place_t2 = true;
                printf("Place YELLOW.\n");
            }
        server_socket.send(msg, *player_1);
        server_socket.send(msg, *player_2);
        break;
        case MessageClient::InputType::FIVE:
                msg.col = 4;
                if (!place_t2)
            {
                place_t2 = true;
                printf("Place YELLOW.\n");
            }
        server_socket.send(msg, *player_1);
        server_socket.send(msg, *player_2);
        break;
        case MessageClient::InputType::SIX:
                msg.col = 5;
                if (!place_t2)
            {
                place_t2 = true;
                printf("Place YELLOW.\n");
            }
        server_socket.send(msg, *player_1);
        server_socket.send(msg, *player_2);
        break;
        case MessageClient::InputType::SEVEN:
                msg.col = 6;
                if (!place_t2)
            {
                place_t2 = true;
                printf("Place YELLOW.\n");
            }
        server_socket.send(msg, *player_1);
        server_socket.send(msg, *player_2);
        break;
        case MessageClient::InputType::PLAY:
            p2_ready = true;
        break;
        default:
        break;
        }
    }

    // reset input
    input_p1 = input_p2 = MessageClient::InputType::NONE;
};

void Server::stepSimulation()
{
    checkGrid();
}

void Server::updateInfoClients()
{
    // MessageServer msg();
    // msg.type = MessageServer::ServerMessageType::UPDATE_INFO;
    // server_socket.send(msg, *player_1);
    // server_socket.send(msg, *player_1);
}

void Server::sendStateMessage()
{
    MessageServer msg(state);
    msg.type = MessageServer::ServerMessageType::UPDATE_STATE;

    if (player_1 != nullptr)
        server_socket.send(msg, *player_1);
    if (player_2 != nullptr)
        server_socket.send(msg, *player_2);

    if (state != MessageServer::ServerState::PLAYING)
        reset();
}

void Server::checkGrid(){
    MessageServer msg(state);
    msg.type = MessageServer::ServerMessageType::CHECK_GRID;

    if (player_1 != nullptr){
        msg.color = "red";
        server_socket.send(msg, *player_1);
    }
    if (player_2 != nullptr){
        msg.color = "yellow";
        server_socket.send(msg, *player_2);
    }
}