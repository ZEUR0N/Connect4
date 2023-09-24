#include "MessageServer.h"

void MessageServer::to_bin() {
    alloc_data(SERVER_MESSAGE_SIZE);
    memset(_data, 0, SERVER_MESSAGE_SIZE);

    char *tmp = _data;

    memcpy(tmp, &type, sizeof(ServerMessageType));
    tmp += sizeof(ServerMessageType);

    memcpy(tmp, &action_, sizeof(ActionType));
    tmp += sizeof(ActionType);

    memcpy(tmp, &state, sizeof(ServerState));
    tmp += sizeof(ServerState);

    memcpy(tmp, &col, sizeof(int));
    tmp += sizeof(int);

    memcpy(&color, tmp, sizeof(std::string));
    tmp += sizeof(std::string);

}

int MessageServer::from_bin(char * bobj)
{
    alloc_data(SERVER_MESSAGE_SIZE);
    memcpy(static_cast<void*>(_data), bobj, SERVER_MESSAGE_SIZE);

    char *tmp = _data;

    memcpy(&type,tmp, sizeof(ServerMessageType));
    tmp += sizeof(ServerMessageType);

    memcpy(&action_, tmp, sizeof(ActionType));
    tmp += sizeof(ActionType);

    memcpy(&state, tmp, sizeof(ServerState));
    tmp += sizeof(ServerState);

    memcpy(&col, tmp, sizeof(int));
    tmp += sizeof(int);

    memcpy(&color, tmp, sizeof(std::string));
    tmp += sizeof(std::string);

    return 0;
}