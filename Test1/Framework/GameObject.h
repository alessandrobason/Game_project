#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "InputHandler.h"
#include "Collision.h"
#include "UsefulFunctions.h"
#include "RoomManager.fwd.h"

class GameObject {
public:
	GameObject() {}
	GameObject(InputHandler* i, RoomManager* rm, sf::RenderWindow* win) {
		collider.setDebugColor(sf::Color::Green);
		in = i;
		roommanager = rm;
		w = win;
	}
	~GameObject() {}

	// Virtual update/draw function.
	virtual void handleInput(float dt) {}
	virtual void update(float dt) {}
	virtual void draw() {}
	virtual void drawDebug() { collider.drawDebug(w); }

	virtual bool animationCallback(std::string name) { return false; }

	virtual sf::Sprite* getSprite() { return nullptr; }

	void move(sf::Vector2f mov) {
		getSprite()->move(mov);
		collider.moveCollision(mov);
		//collider.setDebugPosition(sf::Vector2f(collider.rect.left + 1, collider.rect.top + 1));
	}

	// Set the input component
	void setInput(InputHandler* input) { in = input; }
	void setWindow(sf::RenderWindow* win) { w = win; }
	void setRoomManager(RoomManager* rm) { roommanager = rm; }

	float getY() { return collider.rect.top; }

	virtual void setPosition(sf::Vector2f position) {
		getSprite()->setPosition(position);
		collider.rect = sf::FloatRect(position.x, position.y, collider.rect.width, collider.rect.height);
		collider.setDebugPosition(sf::Vector2f(position.x+1, position.y+1));
	}

	// Control sprite speed
	void setSpeed(float spd) { speed = spd; }
	float getSpeed() { return speed; }

	// Setter/getter for texture component
	void setTexture(sf::Texture* t) { txt = t; }
	sf::Texture* getTexture() { return txt; }

	virtual void hit(float damage) { std::cout << "Object hit\n"; }

	Collision collider;
	sf::Vector2f vel;
	sf::Vector2f oldVel;

	Collision::LAYER collisionlayer = Collision::LAYER::ALL;

protected:
	// Sprite properties
	float speed = 0;

	InputHandler* in = nullptr;
	sf::RenderWindow* w = nullptr;
	// pointer to the room manager
	RoomManager* roommanager = nullptr;

	sf::Texture* txt = nullptr;
};