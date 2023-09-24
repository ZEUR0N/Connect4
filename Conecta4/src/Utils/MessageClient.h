#include <unistd.h>
#include <string.h>
#include <string>
#include <cctype>

#include "Serializable.h"
#include "Vector2D.h"

#define CLIENT_MESSAGE_SIZE sizeof(ClientMessageType) + sizeof(InputType) + (2 * sizeof(int)) + sizeof(float) + (2 * sizeof(Vector2D))

class MessageClient: public Serializable
{
public:
    enum ClientMessageType : uint8_t {
        REGISTER = 0,
        HANDLE_INPUT = 1,
        QUIT = 2
    };

    enum InputType : uint8_t {
        NONE = 0,
        ONE = 1,
        TWO = 2,
        THREE = 3,
        FOUR = 4, 
        FIVE = 5,
        SIX = 6,
        SEVEN = 7,
        PLAY = 8,
        ESCAPE = 9
    };

    MessageClient() {};

    MessageClient(const ClientMessageType& t, const InputType& i)
        : type(t), input(i), win_width(0), win_height(0), win_widthL(0), win_heightT(0) {};

    void setDefaultValues(const int &w_wL, const int &w_w, const int &w_hT, const int &w_h);

    void to_bin();

    int from_bin(char* bobj);

public:
    ClientMessageType type;

    // input value for HANDLE_INPUT
    InputType input;

    // initial values for REGISTER message
    int win_widthL, win_width, win_heightT, win_height;
};