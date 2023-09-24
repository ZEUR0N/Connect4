#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <cassert>

#include <SDL2/SDL.h>

#include "../../Utils/Vector2D.h"

class GameObject {
public:
	GameObject() : tr_(), dim_(), rotation(0), text_(nullptr), textDim_() {
		enabled_ = true;
	}

	virtual ~GameObject();

	virtual void handleInput(const SDL_Event &event) {};
	virtual void update() {};

	inline bool isEnabled() const {
		return enabled_;
	}

	inline bool setEnabled(bool e) {
		return enabled_ = e;
	}

	virtual void render();

	void setTransform(const int x, const int y);

	Vector2D getTransform() const;

	void setTransform(Vector2D tr);

	void setRotation(const float rot);

	void setDimensions(const int w, const int h);

	Vector2D getDimensions() const;

	void setTexture(const std::string& path);

protected:

	Vector2D tr_;
	Vector2D dim_;

	bool enabled_;
	float rotation;

	SDL_Texture* text_;
	Vector2D textDim_;
    
	void render(const SDL_Rect &src, const SDL_Rect &dest,
            const SDL_Point *p = nullptr, SDL_RendererFlip flip = SDL_FLIP_NONE);
};
#endif