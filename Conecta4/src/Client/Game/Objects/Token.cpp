#include "Token.h"
#include <iostream>

Token::Token() : GameObject() {
}

Token::~Token() {
        std::cout << "Token Borrado\n";
}

void Token::setColor(std::string c){
        color = c;
}

std::string Token::getColor(){
        return color;
}