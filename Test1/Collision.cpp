#include "Collision.h"

// RECTANGLE
Collision::Collision() {
	type = RECT;
	rect = sf::IntRect(0, 0, 0, 0);
}

Collision::Collision(sf::IntRect r) {
	type = RECT;
	rect = r;
}

Collision::Collision(int x, int y, int w, int h) {
	type = RECT;
	rect = sf::IntRect(x, y, w, h);
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

//////////////////////////////////////////////////

bool Collision::Check_Collision(sf::IntRect r) {
	switch (type) {
	case RECT:
		return rect.intersects(r);
		break;
	case CIRCLE:
		/*
		int dx = circle.pos.x - c.pos.y;
		int dy = circle.pos.y - c.pos.y;
		float d = std::sqrtf(dx * dx + dy * dy);
		return d < circle.radius - c.radius;
		*/
		return false;
		break;
	}
}

bool Collision::Check_Collision(IntCircle c) {
	switch (type) {
		case RECT:
			return false;
			break;
		case CIRCLE:
			int dx = circle.pos.x - c.pos.y;
			int dy = circle.pos.y - c.pos.y;
			float d = std::sqrtf(dx * dx + dy * dy);
			return d < circle.radius - c.radius;
			break;
	}
}