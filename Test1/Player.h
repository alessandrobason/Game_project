#pragma once
#include <SFML/Graphics.hpp>
#include "JSONparser/JSONparser.h"
#include "Framework/GameObject.h"
#include "Framework/InputHandler.h"
#include "Framework/Collision.h"
#include "Framework/Animation.h"
#include "Framework/Room.h"

class Player : public GameObject {
public:
	enum DIRECTIONS {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};
private:
	// pointer to the parent room to access texture ecc
	Room* room = nullptr;
	
	bool pressed_keys[256] = { false };

	DIRECTIONS last_direction = UP;

	AnimatedSprite animSprite;
	
	JSONparser* config;

public:
	Player();
	Player(InputHandler* input, Room* r, sf::Vector2f pos);

	void handleInput(float dt) override;
	void update(float dt) override;
	void draw(sf::RenderWindow* w) override;

	void setTexture(sf::Texture* txt) {

	}

	void setPosition(sf::Vector2f pos);
	void setDirection(DIRECTIONS dir);
	DIRECTIONS getDirection() { return last_direction; }
	void setCurrentAnimation(std::string anim) { animSprite.setCurrentAnimation(anim); }

	static std::string getGameObjectString() { return ""; }

	sf::Sprite* getSprite() override { return animSprite.getSprite(); };

};

