#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "GameObject.h"
#include "InputHandler.h"
#include "Collision.h"

class Player : public GameObject{
private:
	sf::Texture spritesheet;
	sf::RectangleShape collisionShape = sf::RectangleShape();

	bool pressed_keys[256] = { false };

public:
	Player();
	Player(InputHandler* input, std::string txt_path, sf::Vector2f pos);

	void handleInput(float dt) override;
	void update(float dt) override;
	void draw(sf::RenderWindow* w) override;

	void move(sf::Vector2f mov);

	sf::Sprite sprite;

	Collision collision = Collision();
	
	sf::Vector2f vel;
	sf::Vector2f oldVel;
};

