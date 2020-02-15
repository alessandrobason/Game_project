#pragma once
#include <SFML/Graphics.hpp>
#include "JSONparser/JSONparser.h"
#include "Framework/GameObject.h"
#include "Framework/InputHandler.h"
#include "Framework/Collision.h"
#include "Framework/AnimatedSprite.h"
#include "Framework/Room.h"

class Player : public GameObject {
private:
	// pointer to the parent room to access texture ecc
	Room* room = nullptr;

	sf::RectangleShape collisionShape = sf::RectangleShape();

	bool pressed_keys[256] = { false };

	enum SPRITE_ANIMATIONS {
		IDLE_U = 0,
		IDLE_D = 1,
		IDLE_L = 2,
		IDLE_R = 3,
		WALK_U = 4,
		WALK_D = 5,
		WALK_L = 6,
		WALK_R = 7
	};

	AnimatedSprite animSprite;
	
	JSONparser* config;

public:
	Player();
	Player(InputHandler* input, Room* r, std::string txt_path, sf::Vector2f pos);

	void handleInput(float dt) override;
	void update(float dt) override;
	void draw(sf::RenderWindow* w) override;
	void drawDebug(sf::RenderWindow* w) override;

	float getY() override { return getSprite()->getPosition().y; };

	void move(sf::Vector2f mov);

	sf::Sprite* getSprite() { return animSprite.getSprite(); };
	
	Collision collision = Collision();
	
	sf::Vector2f vel;
	sf::Vector2f oldVel;
};

