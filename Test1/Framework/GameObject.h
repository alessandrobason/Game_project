#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "InputHandler.h"

class GameObject {
public:
	GameObject();
	~GameObject();

	// Virtual update/draw function.
	virtual void handleInput(float dt);
	virtual void update(float dt);
	virtual void draw(sf::RenderWindow* w);

	// Set the input component
	void setInput(InputHandler* input) { in = input; };

	// Control sprite speed
	void setSpeed(float spd) { speed = spd; }
	float getSpeed() { return speed; }

protected:
	// Sprite properties
	float speed = 0;

	// input component
	InputHandler* in = nullptr;
};