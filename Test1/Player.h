#pragma once
#include <SFML/Graphics.hpp>
#include "JSONparser/JSONparser.h"
#include "Framework/GameObject.h"
#include "Framework/InputHandler.h"
#include "Framework/Collision.h"
#include "Framework/Animation.h"
#include "Framework/RoomManager.fwd.h"

class Player : public GameObject {
public:
	enum DIRECTIONS {
		UP,
		DOWN,
		LEFT,
		RIGHT
	};
private:
	// pointer to the room manager to access texture ecc
	RoomManager* roommanager = nullptr;
	
	bool pressed_keys[256] = { false };

	DIRECTIONS last_direction = UP;

	AnimatedSprite animSprite;
	
	JSONparser* config;

public:
	Player();
	Player(InputHandler* input, RoomManager* rm, sf::RenderWindow* win);

	void handleInput(float dt) override;
	void update(float dt) override;
	void draw() override;

	void setPosition(sf::Vector2f pos);
	void setDirection(int dir);
	DIRECTIONS getDirection() { return last_direction; }
	void setCurrentAnimation(std::string anim) { animSprite.setCurrentAnimation(anim); }

	static std::string getGameObjectString() { return ""; }

	sf::Sprite* getSprite() override { return animSprite.getSprite(); };

};

