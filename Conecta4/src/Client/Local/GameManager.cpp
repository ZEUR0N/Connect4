#include "GameManager.h"
#include "../SDL_Utils/App.h"

#include <cassert>

GameManager::GameManager(): scenerioLimits_(), relativeLimits_() {
}

GameManager::~GameManager() {
}

Vector2D GameManager::getScenerioLimits() {
	return scenerioLimits_;
}

Vector2D GameManager::getRelativeScenerioLimits() {
	return relativeLimits_;
}

void GameManager::setScenerioLimits(Vector2D limit) {
		scenerioLimits_ = limit;
		relativeLimits_ = Vector2D(App::instance()->width() - scenerioLimits_.getX(),App::instance()->height() - scenerioLimits_.getY());
}