#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "InputHandler.h"
#include "Collision.h"

class GameObject {
public:
	GameObject(InputHandler* i, sf::RenderWindow* win) {
		collider.setDebugColor(sf::Color::Green);
		in = i;
		w = win;
	}
	~GameObject() {}

	// Virtual update/draw function.
	virtual void handleInput(float dt) {}
	virtual void update(float dt) {}
	virtual void draw() {}
	virtual void drawDebug() { collider.drawDebug(w); }

	virtual sf::Sprite* getSprite() { return nullptr; }

	void move(sf::Vector2f mov) {
		getSprite()->move(mov);
		collider.moveCollision(mov);
		//collider.setDebugPosition(sf::Vector2f(collider.rect.left + 1, collider.rect.top + 1));
	}

	// Set the input component
	void setInput(InputHandler* input) { in = input; }

	float getY() { return collider.rect.top; }

	// Control sprite speed
	void setSpeed(float spd) { speed = spd; }
	float getSpeed() { return speed; }

	// Setter/getter for texture component
	void setTexture(sf::Texture* t) { txt = t; }
	sf::Texture* getTexture() { return txt; }

	virtual void hit() { std::cout << "Object hit\n"; }

	Collision collider;
	sf::Vector2f vel;
	sf::Vector2f oldVel;

	Collision::LAYER collisionlayer = Collision::LAYER::ALL;

protected:
	// Sprite properties
	float speed = 0;

	// input component
	InputHandler* in = nullptr;
	sf::RenderWindow* w = nullptr;

	sf::Texture* txt = nullptr;
};