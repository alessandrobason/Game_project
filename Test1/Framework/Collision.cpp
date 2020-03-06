#include "Collision.h"
#include <iostream>

Collision::Collision() {
	rect = sf::FloatRect(0, 0, 0, 0);
	collision_offset = sf::Vector2f();
	setupDebugBox(rect);
}

Collision::Collision(sf::FloatRect r, LAYER t) {
	collisionlayer = t;
	rect = r;
	collision_offset = sf::Vector2f(r.left, r.top);
	setupDebugBox(rect);
}

Collision::Collision(int x, int y, int w, int h, LAYER t) {
	collisionlayer = t;
	rect = sf::FloatRect(x, y, w, h);
	collision_offset = sf::Vector2f(x, y);
	setupDebugBox(rect);
}

void Collision::setupDebugBox(sf::FloatRect r) {
	debug.setPosition(sf::Vector2f(r.left + 1, r.top + 1));
	debug.setSize(sf::Vector2f(r.width - 2, r.height - 2));
	debug.setOutlineColor(sf::Color::Red);
	debug.setOutlineThickness(1.f);
	debug.setFillColor(sf::Color::Transparent);
}

Collision::~Collision() {}

//////////////////////////////////////////////////

bool Collision::Check_Collision(sf::FloatRect r) {
	return rect.intersects(r) && r!=rect;
}

sf::Vector2f Collision::getCollisionSide(sf::FloatRect r, sf::Vector2f& oldVel) {
	sf::Vector2f reverseVel = sf::Vector2f(0, 0);
	
	// if left side is after half of rectangle (coming from RIGHT)
	if (rect.left < r.left + r.width && rect.left >= r.left + (r.width / 2)) {
		reverseVel.x = r.left + r.width - rect.left;
	}
	// if left side is before half of rectangle (coming from LEFT)
	else {
		reverseVel.x -= rect.left + rect.width - r.left;
	}
	// if top side is after half of rectangle (coming from BOTTOM)
	if (rect.top < r.top + r.height && rect.top >= r.top + (r.height/2)) {
		reverseVel.y = r.top + r.height - rect.top;
	}
	// if top side is after half of rectangle (coming from TOP)
	else {
		reverseVel.y -= rect.top + rect.height - r.top;
	}

	if (std::abs(reverseVel.x) > 1)		reverseVel.x = 0;
	else if(std::abs(reverseVel.y) > 1)	reverseVel.y = 0;

	return reverseVel;
}

void Collision::moveCollision(sf::Vector2f mov) {
	rect.left += mov.x;
	rect.top += mov.y;
	debug.setPosition(sf::Vector2f(rect.left + 1, rect.top + 1));
}

void Collision::drawDebug(sf::RenderWindow* w) {
	//debug.setPosition(sf::Vector2f(rect.left, rect.top));
	w->draw(debug);
}
