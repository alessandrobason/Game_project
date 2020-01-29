#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "InputHandler.h"

class Player {
public:
	Player();
	Player(InputHandler* input, std::string txt_path, sf::Vector2f pos);

	void Update(float dt);

	void move(sf::Vector2f mov);

	sf::Sprite sprite;

private:
	sf::Texture spritesheet;
	InputHandler *in;

	float speed = 1.5f;

	bool pressed_keys[256] = { false };
};

