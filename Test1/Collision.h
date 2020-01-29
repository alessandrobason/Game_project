#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>

class Collision {
private:
	enum COLLISION_TYPES {
		RECT,
		CIRCLE
	};

	struct IntCircle {
		sf::Vector2i pos;
		int radius;
	};

	COLLISION_TYPES type;
	sf::IntRect rect;
	IntCircle circle;

public:
	// RECTANGLE (by default it's a 0x0 rectangle) 
	Collision();
	Collision(sf::IntRect r);
	Collision(int x, int y, int w, int h);

	// CIRCLE
	Collision(sf::Vector2i p, int r);
	Collision(int x, int y, int r);
	~Collision();

	bool Check_Collision(sf::IntRect r);
	bool Check_Collision(IntCircle c);
};

