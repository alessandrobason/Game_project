#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "InputHandler.h"
#include "Collision.h"

class Player {
private:
	sf::Texture spritesheet;
	InputHandler* in;

	sf::RectangleShape collisionShape = sf::RectangleShape();

	float speed = 100.0f;

	bool pressed_keys[256] = { false };

public:
	Player();
	Player(InputHandler* input, std::string txt_path, sf::Vector2f pos);

	void handleInput(float dt);
	void update(float dt);
	void draw(sf::RenderWindow* w);


	void move(sf::Vector2f mov);

	sf::Sprite sprite;

	Collision::Colliding colliding = { false, false, false, false };
	Collision collision = Collision();
	
	sf::Vector2f vel;
};

