#include "MessageClient.h"

void MessageClient::setDefaultValues(const int &w_wL, const int &w_w, const int &w_hT, const int &w_h)
{
    win_widthL = w_wL; win_heightT = w_hT;
    win_width = w_w; win_height = w_h;
};

void MessageClient::to_bin()
{
    alloc_data(CLIENT_MESSAGE_SIZE);
    memset(_data, 0, CLIENT_MESSAGE_SIZE);

    char *tmp = _data;

    memcpy(tmp, &type, sizeof(ClientMessageType));
    tmp += sizeof(ClientMessageType);

    memcpy(tmp, &input, sizeof(InputType));
    tmp += sizeof(InputType);

    memcpy(tmp, &win_width, sizeof(int));
    tmp += sizeof(int);

    memcpy(tmp, &win_height, sizeof(int));
    tmp += sizeof(int);

    memcpy(tmp, &win_widthL, sizeof(int));
    tmp += sizeof(int);

    memcpy(tmp, &win_heightT, sizeof(int));
    tmp += sizeof(int);
}

int MessageClient::from_bin(char *bobj)
{
    alloc_data(CLIENT_MESSAGE_SIZE);
    memcpy(static_cast<void *>(_data), bobj, CLIENT_MESSAGE_SIZE);

    char *tmp = _data;

    memcpy(&type, tmp, sizeof(ClientMessageType));
    tmp += sizeof(ClientMessageType);

    memcpy(&input, tmp, sizeof(InputType));
    tmp += sizeof(InputType);

    memcpy(&win_width, tmp, sizeof(int));
    tmp += sizeof(int);

    memcpy(&win_height, tmp, sizeof(int));
    tmp += sizeof(int);

    memcpy(tmp, &win_widthL, sizeof(int));
    tmp += sizeof(int);

    memcpy(tmp, &win_heightT, sizeof(int));
    tmp += sizeof(int);

    return 0;
}