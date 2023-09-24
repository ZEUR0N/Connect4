#ifndef TOKEN_H_
#define TOKEN_H_

#include "../../SDL_Utils/GameObject.h"
#include <vector>
#include <string>
#include <array>

class Token: public GameObject {
public:
    Token();
    virtual ~Token();

    void setColor(std::string c);
    std::string getColor();
private:
    std::string color;

};
#endif