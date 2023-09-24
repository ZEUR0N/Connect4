#include "Grid.h"

#include "../../SDL_Utils/App.h"
#include "../../Local/GameManager.h"


Grid::Grid(Client* game) : GameObject(){
	app_ = game;

	for(int i = 0; i < 7; i++){
		for(int j = 0; j < 6; j++){
			tokenGrid[i][j] = nullptr;
		}
	}

    setKeys();
}

Grid::~Grid() {}

void Grid::handleInput(const SDL_Event &e) {

	if (e.type == SDL_KEYDOWN){
		key = MessageClient::InputType::NONE; 

		if (e.key.keysym.scancode == one){
			key = MessageClient::InputType::ONE; 
		}
		else if (e.key.keysym.scancode == two){
			key = MessageClient::InputType::TWO;
		}
		else if (e.key.keysym.scancode == three){
			key = MessageClient::InputType::THREE;
		}
		else if (e.key.keysym.scancode == four){
			key = MessageClient::InputType::FOUR;
		}
		else if(e.key.keysym.scancode == five){
			key = MessageClient::InputType::FIVE;
		}
		else if(e.key.keysym.scancode == six){
			key = MessageClient::InputType::SIX;
		}
		else if(e.key.keysym.scancode == seven){
			key = MessageClient::InputType::SEVEN;
		}

		app_->sendGameMessage(key);
	}
}

void Grid::render(){
	GameObject::render();

	for(int i = 0; i < 7; i++){
		for(int j = 0; j < 6; j++){
			if(tokenGrid[i][j] != nullptr)
				tokenGrid[i][j]->render();
		}
	}
}

void Grid::setKeys()
{
	one = SDL_SCANCODE_1;
	two = SDL_SCANCODE_2;
	three = SDL_SCANCODE_3;
	four = SDL_SCANCODE_4;
	five = SDL_SCANCODE_5;
	six = SDL_SCANCODE_6;
	seven = SDL_SCANCODE_7;
}


void Grid::placeToken(Token* token, int col){
    //Ponemos el token en la posicion mas baja del tablero de esa columna
    int freeRow = 5;
    while(freeRow >= 0 && tokenGrid[col][freeRow] != nullptr) freeRow--;
    if(freeRow >= 0){
        token->setTransform(120 + 33 + 88 * col, 90 + 30 + 88 * freeRow);
        tokenGrid[col][freeRow] = token;
        printf("Token Placed");
    }
}

bool Grid::checkPlayerTokens(std::string color){
    bool conecta4 = false;

    //Vertical
    for(int i = 0; i < 7; i++){
        for(int j = 0; j < 3; j++){//hasta 3 pq es de arriba a abajo
            if(!conecta4){
                if(tokenGrid[i][j] != nullptr){
                    if(tokenGrid[i][j]->getColor() == color){
                        int r = j + 1;
                        int seguidos = 1;
                        while(r < 6 && tokenGrid[i][r] != nullptr){
                            if(tokenGrid[i][r]->getColor() == color){

                                r++;
                                seguidos++;
                            }
                            else r = 99;
                        }
                        if(seguidos >= 4)
                            conecta4 = true;
                    }

                }
            }
        }
    }
    //Horizontal
    if(!conecta4){
        for(int i = 0; i < 4; i++){//hasta 4 pq es de izquierda a derecha
            for(int j = 0; j < 6; j++){
                if(!conecta4){
                    if(tokenGrid[i][j] != nullptr){
                        if(tokenGrid[i][j]->getColor() == color){
                            int c = i + 1;
                            int seguidos = 1;
                            while(c < 6 && tokenGrid[c][j] != nullptr){
                                if(tokenGrid[c][j]->getColor() == color){
                                    c++;
                                    seguidos++;
                                }
                                else c = 99;
                            }
                            if(seguidos >= 4)
                                conecta4 = true;
                        }

                    }
                }
            }
        }
    }
    //Diagonal hacia derecha
    if(!conecta4){
        for(int i = 0; i < 4; i++){//hasta 4 pq es de izquierda a derecha
            for(int j = 0; j < 3; j++){//hasta 3 pq es de arriba a abajo
                if(!conecta4){
                    if(tokenGrid[i][j] != nullptr){
                        if(tokenGrid[i][j]->getColor() == color){
                            int r = j + 1;
                            int c = i + 1;
                            int seguidos = 1;
                            while((r < 6 || c < 7) && tokenGrid[c][r] != nullptr){
                                if(tokenGrid[c][r]->getColor() == color){
                                    r++;
                                    c++;
                                    seguidos++;
                                }
                                else r = 99;
                            }
                            if(seguidos >= 4)
                                conecta4 = true;
                        }

                    }
                }
            }
        }
    }
    //Diagonal hacia izquierda
    if(!conecta4){
        for(int i = 6; i > 2; i--){//hasta 2 pq es de derecha a izquierda
            for(int j = 5; j > 2; j--){//hasta 2 pq es de abajo a arriba
                if(!conecta4){
                    if(tokenGrid[i][j] != nullptr){
                        if(tokenGrid[i][j]->getColor() == color){
                            int r = j - 1;
                            int c = i - 1;
                            int seguidos = 1;
                            while((r >= 0 || c >= 0) && tokenGrid[c][r] != nullptr){
                                if(tokenGrid[c][r]->getColor() == color){
                                    r--;
                                    c--;
                                    seguidos++;
                                }
                                else r = -99;
                            }
                            if(seguidos >= 4)
                                conecta4 = true;
                        }

                    }
                }
            }
        }
    }

    return conecta4;
}

void Grid::setColor(std::string c){
    color = color;
}

std::string Grid::getColor(){
    return color;
}