#pragma once
#include <SFML/Graphics.hpp>
#include "JSONparser/JSONparser.h"
#include "Framework/GameObject.h"
#include "Framework/InputHandler.h"
#include "Framework/Collision.h"
#include "Framework/Animation.h"
#include "Framework/RoomManager.fwd.h"
#include "Framework/Weapon.h"

class Player : public GameObject {
public:
	enum DIRECTIONS {
		UP_RIGHT,
		DOWN_RIGHT,
		DOWN_LEFT,
		UP_LEFT,
	};
private:
	// pointer to the room manager to access texture ecc
	RoomManager* roommanager = nullptr;

	JSONparser* config;
	
	bool pressed_keys[256] = { false };

	AnimatedSprite animSprite;
	DIRECTIONS last_direction = DOWN_RIGHT;

	sf::Vector2f local_center;
	float angle_to_mouse = 0.f;

	Weapon bow;
	bool draw_weapon_over = false;

public:
	Player();
	Player(InputHandler* input, RoomManager* rm, sf::RenderWindow* win);

	void handleInput(float dt) override;
	void update(float dt) override;
	void draw() override;
	void drawDebug() override;

	void move(sf::Vector2f offset);

	void setPosition(sf::Vector2f pos);
	void setDirection(int dir);
	DIRECTIONS getDirection() { return last_direction; }
	void setCurrentAnimation(std::string anim) { animSprite.setCurrentAnimation(anim); }
	sf::Vector2f getLocalCenter() { return local_center; }

	static std::string getGameObjectString() { return ""; }

	sf::Sprite* getSprite() override { return animSprite.getSprite(); };

};

