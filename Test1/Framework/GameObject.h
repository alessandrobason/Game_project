#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "InputHandler.h"
#include "Collision.h"

class GameObject {
public:
	GameObject() {
		collider.setDebugColor(sf::Color::Green);
	}
	~GameObject() {}

	// Virtual update/draw function.
	virtual void handleInput(float dt) {}
	virtual void update(float dt) {}
	virtual void draw(sf::RenderWindow* w) {}
	void drawDebug(sf::RenderWindow* w) { collider.drawDebug(w); }

	virtual sf::Sprite* getSprite() { return nullptr; }

	void move(sf::Vector2f mov) {
		getSprite()->move(mov);
		collider.rect.left = getSprite()->getGlobalBounds().left;
		collider.rect.top = getSprite()->getGlobalBounds().top;
		collider.setDebugPosition(sf::Vector2f(collider.rect.left + 1, collider.rect.top + 1));
	}

	// Set the input component
	void setInput(InputHandler* input) { in = input; }

	virtual float getY() { return 0.f; }

	// Control sprite speed
	void setSpeed(float spd) { speed = spd; }
	float getSpeed() { return speed; }

	// Setter/getter for texture component
	void setTexture(sf::Texture* t) { txt = t; }
	sf::Texture* getTexture() { return txt; }

	Collision collider;
	sf::Vector2f vel;
	sf::Vector2f oldVel;

protected:
	// Sprite properties
	float speed = 0;

	// input component
	InputHandler* in = nullptr;

	sf::Texture* txt = nullptr;
};