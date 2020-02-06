#include "Collision.h"
#include <iostream>

// RECTANGLE
Collision::Collision() {
	type = RECT;
	rect = sf::FloatRect(0, 0, 0, 0);
}

Collision::Collision(sf::FloatRect r) {
	type = RECT;
	rect = r;
}

Collision::Collision(int x, int y, int w, int h) {
	type = RECT;
	rect = sf::FloatRect(x, y, w, h);
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

Collision::~Collision()
{
}

//////////////////////////////////////////////////

bool Collision::Check_Collision(sf::FloatRect r) {
	switch (type) {
	case RECT:
	{
		/*
		bool coll = false;
		if (rect.top + rect.height > r.top && rect.top < r.top + r.height) {
			if (rect.left + rect.width == r.left) {
				collision_side = E;
				coll = true;
			}
			if (rect.left + 1 == r.left + r.width) {
				collision_side = W;
				coll = true;
			}
		}

		if (rect.left + rect.width > r.left && rect.left < r.left + r.width) {
			if (rect.top + rect.height == r.top) {
				collision_side = S;
				coll = true;
			}
			if (rect.top + 1 == r.top + r.height) {
				collision_side = N;
				coll = true;
			}
		}

		return coll;
		if (rect.left == r.left + r.width && rect.top < r.top+r.height && rect.top+rect.height > r.top) {
			collision_side = W;
			std::cout << "West\n";
			return true;
		}
		if (rect.left == r.left + r.width && rect.top < r.top + r.height && rect.top + rect.height > r.top) {
			collision_side = W;
			std::cout << "West\n";
			return true;
		}
		/*
		if (rect.left < r.left + r.width && r.left < rect.left+rect.width &&
			rect.top < r.top + r.height && rect.top + rect.height > r.top) {
			return true;
		}

		r.left--;
		r.top--;
		r.width++;
		r.height++;
		if (rect.left+1 < r.left + r.width && r.left < rect.left + rect.width &&
			rect.top < r.top + r.height && rect.top + rect.height > r.top) {
			return true;
		}
		*/

		return rect.intersects(r);
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