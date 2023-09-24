#include "Client.h"

#include "../SDL_Utils/GameObject.h"
#include "../SDL_Utils/macros.h"
#include "../SDL_Utils/App.h"

#include "../Game/Font.h"
#include "../Game/Background.h"
#include "../Game/Objects/Grid.h"
#include "../Game/Objects/Token.h"

#include "GameManager.h"

Client::Client(const char *s, const char *p) : client_socket(s, p), player_(nullptr),
								currentState(MessageServer::ServerState::WAITING), nextState(MessageServer::ServerState::EMPTY), gridState(GridState::NONE){};

Client::~Client() {}

void Client::net_message_thread()
{
	MessageServer server_recv_msg;
	Socket *net_socket = new Socket(client_socket);
	while (true)
	{
		client_socket.recv(server_recv_msg, net_socket);
		switch (server_recv_msg.type)
		{
		case MessageServer::ServerMessageType::UPDATE_STATE:
			nextState = server_recv_msg.state;
			break;
		case MessageServer::ServerMessageType::CHECK_GRID:
		{
		if(player_ != nullptr && nextState ==  MessageServer::ServerState::EMPTY && currentState ==  MessageServer::ServerState::PLAYING){
			player_->setColor(server_recv_msg.color);
			color = server_recv_msg.color;
			if(player_->checkPlayerTokens("red")){
				printf("RED_WINS\n");
				gridState = GridState::RED;
			}
			else if(player_->checkPlayerTokens("yellow")){
				printf("YELLOW_WINS\n");
				gridState = GridState::YELLOW;
			}
			}
		}
		break;
		case MessageServer::ServerMessageType::ACTION:
		{
			switch (server_recv_msg.action_)
			{
			case MessageServer::ActionType::PLACE_RED_TOKEN:
				placeToken("red", server_recv_msg.col);
				break;
			
			case MessageServer::ActionType::PLACE_YELLOW_TOKEN: 
				placeToken("yellow", server_recv_msg.col);
				break;
		}
		}
		break;
	}
}
}

void Client::init(int w, int h)
{
	App::init("Connect 4", w, h);
	GameManager::init();

	InitData data;
	data.dim = Vector2D(GRID_WIDTH_SIZE, GRID_HEIGHT_SIZE);
	data.rot = 0;

	changeState(currentState);

	// init connection
	std::thread([this]()
				{ net_message_thread(); })
		.detach();

	sendMatchMessage(MessageClient::ClientMessageType::REGISTER, &data);
	std::cout << "Trying to log...\n";
}

void Client::run()
{
	printf("Client running\n");
	SDL_Event event;

	// animation loop
	while (nextState != MessageServer::ServerState::SERVER_QUIT)
	{
		checkState();

		Uint32 startTime = app().currRealTime();
		while (SDL_PollEvent(&event))
		{
			//Salir del server
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)) {
				currentState = MessageServer::ServerState::SERVER_QUIT;
				continue;
			}

			if ((event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_RETURN)) {
				std::cout << "Lets Begin Game\n";
				sendGameMessage(MessageClient::InputType::PLAY);
				continue;
			}

			if(player_ != nullptr)player_->handleInput(event);
		}

		//Si alguno gana termina
		if(gridState != GridState::NONE && currentState ==  MessageServer::ServerState::PLAYING){
			nextState = MessageServer::ServerState::GAME_OVER;
		}

		if (currentState == MessageServer::ServerState::SERVER_QUIT){
			std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		 	break;
			 }

		// update
		for (auto &o : objs_)
			if (o->isEnabled())
				o->update();

		refresh();

		app().clearRenderer({0, 0, 0});

		// render
		for (auto &o : objs_)
			if (o->isEnabled())
				o->render();

		app().presentRenderer();

		Uint32 frameTime = app().currRealTime() - startTime;
		if (frameTime < 20)
			SDL_Delay(20 - frameTime);
	}

	sendMatchMessage(MessageClient::ClientMessageType::QUIT);
	std::cout << "Quitting Game...\n";
}

void Client::shutdown()
{
	clearGameObjects();
}

void Client::refresh()
{
	objs_.erase(std::remove_if(objs_.begin(), objs_.end(),
							   [](const GameObject *e)
							   {
								   if (e->isEnabled())
									   return false;
								   else
								   {
									   delete e;
									   return true;
								   }
							   }),
				objs_.end());
}

void Client::placeToken(std::string color_, int col) {
	Token* token = new Token();
	token->setDimensions(50, 50);
	if(color_ == "red"){
		token->setTexture("./resources/images/red.png");
	}
	else{
		token->setTexture("./resources/images/yellow.png");
	}

	token->setColor(color_);

	player_->placeToken(token, col);

	objs_.push_back(token);
}


void Client::sendMatchMessage(MessageClient::ClientMessageType msg, InitData *data)
{	
	MessageClient login;
	login.type = msg;

	if (data != nullptr)
		login.setDefaultValues(GameManager::instance()->getRelativeScenerioLimits().getX(), //Left
							  GameManager::instance()->getScenerioLimits().getX(), //Right
							  GameManager::instance()->getRelativeScenerioLimits().getY(),//Top 
							  GameManager::instance()->getScenerioLimits().getY() + 45);

	client_socket.send(login, client_socket);
	printf("Sending Match Message...\n");
}

void Client::sendGameMessage(MessageClient::InputType input)
{
	MessageClient login;
	login.type = MessageClient::ClientMessageType::HANDLE_INPUT;
	login.input = input;
	client_socket.send(login, client_socket);
}

void Client::checkState()
{
	if (nextState != MessageServer::ServerState::EMPTY && currentState != nextState)
	{
		changeState(nextState); // this makes it so there's no problems with thread

		currentState = nextState;
		nextState = MessageServer::ServerState::EMPTY;

		printf("Curr St: %d -- Next St: %d \n", (int)currentState, (int)nextState);
	}
}

void Client::changeState(MessageServer::ServerState state)
{
	clearGameObjects();

	switch (state)
	{
	case MessageServer::ServerState::WAITING:
		loadScreen("./resources/images/title.png", "./resources/fonts/Capture_it.ttf", "Waiting match...",
				   Vector2D(50, 600), Vector2D(1000, 50), {255, 255, 255}, 50);
		break;
	case MessageServer::ServerState::READY:
		loadScreen("./resources/images/title.png", "./resources/fonts/Capture_it.ttf", "Press ENTER to play",
				   Vector2D(200, 600), Vector2D(700, 50), {125, 125, 125}, 50);
		break;
	case MessageServer::ServerState::PLAYING:
		playLoad();
		break;
	case MessageServer::ServerState::GAME_OVER:
	{
		std::string res;
		if(color == "red" && gridState == GridState::RED)
			res = "./resources/images/win.png";
		else if(color == "red" && gridState == GridState::YELLOW)
			res = "./resources/images/lose.png";
		else if(color == "yellow" && gridState == GridState::YELLOW)
			res = "./resources/images/win.png";
		else if(color == "yellow" && gridState == GridState::RED)
			res = "./resources/images/lose.png";
		else
			res = "./resources/images/title.png";

		if(gridState == GridState::RED)
			loadScreen(res, "./resources/fonts/Capture_it.ttf", "Red Wins!",
					Vector2D(300, 600), Vector2D(400, 50), {125, 125, 125}, 50);
		else if(gridState == GridState::YELLOW)
			loadScreen(res, "./resources/fonts/Capture_it.ttf", "Yellow Wins!",
						Vector2D(300, 600), Vector2D(500, 50), {125, 125, 125}, 50);
	}
		break;
	default:
		break;
	}
}

void Client::loadScreen(const std::string &textFile, const std::string &fontFile, const std::string &text,
							  const Vector2D &textPos, const Vector2D &textDim, const SDL_Color &color, const int &size)
{
	Background *bG = new Background();
	bG->setTransform(0, 0);
	bG->setDimensions(app().width(), app().height());
	bG->setTexture(textFile);

	objs_.push_back(bG);

	Font *f = new Font(fontFile, text, color, size);
	f->setTransform(textPos.getX(), textPos.getY());
	f->setDimensions(textDim.getX(), textDim.getY());

	objs_.push_back(f);
}

void Client::playLoad()
{
	printf("Game Starts\n");
	Background *bG = new Background();
	bG->setTransform(0, 0);
	bG->setDimensions(app().width(), app().height());
	bG->setTexture("./resources/images/background.png");

	objs_.push_back(bG);

	player_ = new Grid(this);
	player_->setTransform(120, 90);
	player_->setDimensions(GRID_WIDTH_SIZE, GRID_HEIGHT_SIZE);
	player_->setTexture("./resources/images/grid.png");

	objs_.push_back(player_);
	
	Font *f = new Font("./resources/fonts/Capture_it.ttf", player_->getColor(),{125, 125, 125}, 50);
	f->setTransform(10, 10);
	f->setDimensions(50, 40);

	objs_.push_back(f);


	for(int i = 0; i < gObjsToAdd_.size(); i++)
		objs_.push_back(gObjsToAdd_[i]);
	gObjsToAdd_.clear();
}

void Client::clearGameObjects()
{
	for (unsigned int i = 0; i < objs_.size(); i++){
		delete objs_[i];
		objs_[i] = nullptr;
	}
	objs_.clear();
}