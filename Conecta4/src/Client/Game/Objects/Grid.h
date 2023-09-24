#ifndef GRID_H_
#define GRID_H_

#include "../../SDL_Utils/GameObject.h"
#include "Token.h"
#include "../../Local/Client.h"

#include <vector>


#define OFFSET_X 90
#define OFFSET_Y 60

class Grid: public GameObject {
public:
    Grid(Client* game);
    virtual ~Grid();

    void handleInput(const SDL_Event &) override;


    virtual void render();

    bool checkPlayerTokens(std::string color);
    void placeToken(Token* token, int col);

    void setColor(std::string c);
    std::string getColor();

private:
    Client* app_;
    void setKeys();

    Token* tokenGrid[7][6];
    MessageClient::InputType key;
	SDL_Scancode one, two, three, four, five, six, seven;

    std::string color;
};
#endif