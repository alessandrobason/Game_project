#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "InputHandler.h"

class GameObject {
public:
	GameObject() {}
	~GameObject() {
		delete in;
		in = NULL;
	}

	// Virtual update/draw function.
	virtual void handleInput(float dt) {}
	virtual void update(float dt) {}
	virtual void draw(sf::RenderWindow* w) {}
	virtual void drawDebug(sf::RenderWindow* w) {}

	// Set the input component
	void setInput(InputHandler* input) { in = input; }

	virtual float getY() { return 0.f; }

	// Control sprite speed
	void setSpeed(float spd) { speed = spd; }
	float getSpeed() { return speed; }

	// Setter/getter for texture component
	void setTexture(sf::Texture* t) { txt = t; }
	sf::Texture* getTexture() { return txt; }

protected:
	// Sprite properties
	float speed = 0;

	// input component
	InputHandler* in = nullptr;
	sf::Texture* txt = nullptr;
};