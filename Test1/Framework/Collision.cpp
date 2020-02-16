#include "Collision.h"
#include <iostream>

// RECTANGLE
Collision::Collision() {
	type = RECT;
	rect = sf::FloatRect(0, 0, 0, 0);
	setupDebugBox(rect);
}

Collision::Collision(sf::FloatRect r) {
	type = RECT;
	rect = r;
	setupDebugBox(rect);
}

Collision::Collision(int x, int y, int w, int h) {
	type = RECT;
	rect = sf::FloatRect(x, y, w, h);
	setupDebugBox(rect);
}

void Collision::setupDebugBox(sf::FloatRect r) {
	debug.setPosition(sf::Vector2f(r.left, r.top));
	debug.setSize(sf::Vector2f(r.width, r.height));
	debug.setOutlineColor(sf::Color::Red);
	debug.setOutlineThickness(1.f);
	debug.setFillColor(sf::Color::Transparent);
}

// CIRCLE
Collision::Collision(sf::Vector2i p, int r) {
	type = CIRCLE;
	circle = { p, r };
}

Collision::Collision(int x, int y, int r) {
	type = CIRCLE;
	circle = { sf::Vector2i(x, y), r };
}

Collision::~Collision() {}

//////////////////////////////////////////////////

bool Collision::Check_Collision(sf::FloatRect r) {
	switch (type) {
	case RECT:
	{
		collide = rect.intersects(r);
		return collide;
		break;
	}
	case CIRCLE:
		/*
		int dx = circle.pos.x - c.pos.y;
		int dy = circle.pos.y - c.pos.y;
		float d = std::sqrtf(dx * dx + dy * dy);
		return d < circle.radius - c.radius;
		*/
		break;
	}
	return false;
}

bool Collision::Check_Collision(IntCircle c) {
	switch (type) {
		case RECT:
			break;
		case CIRCLE:
			int dx = circle.pos.x - c.pos.y;
			int dy = circle.pos.y - c.pos.y;
			float d = std::sqrt(dx * dx + dy * dy);
			return d < circle.radius - c.radius;
			break;
	}
	return false;
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

void Collision::drawDebug(sf::RenderWindow* w) {
	//debug.setPosition(sf::Vector2f(rect.left, rect.top));
	w->draw(debug);
}
