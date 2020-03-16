#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "InputHandler.h"
#include "Collision.h"
#include "UsefulFunctions.h"
#include "RoomManager.fwd.h"
#include "FiniteStateMachine.h"

class GameObject : public FiniteStateMachine {
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

	virtual void isPlayerInside(bool p) {}

	virtual sf::Sprite* getSprite() { return nullptr; }

	virtual void move(sf::Vector2f mov) {
		getSprite()->move(mov);
		collider.moveCollision(mov);
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

	sf::Vector2f getCenter() {
		sf::Vector2f c;
		c.x = collider.rect.left + collider.rect.width / 2;
		c.y = collider.rect.top + collider.rect.height / 2;
		return c;
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
	bool collided = false;

	Collision checkbox;

	Collision::LAYER collisionlayer = Collision::LAYER::ALL;

protected:
	// Sprite properties
	float speed = 0;

	bool dead = false;

	InputHandler* in = nullptr;
	sf::RenderWindow* w = nullptr;
	// pointer to the room manager
	RoomManager* roommanager = nullptr;

	sf::Texture* txt = nullptr;

	sf::Vector2f checkboxsize = sf::Vector2f(0, 0);
};