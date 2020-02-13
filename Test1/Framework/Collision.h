#pragma once
#include <SFML/Graphics.hpp>
#include <math.h>

class Collision {
private:
	bool collide = false;

	enum COLLISION_SHAPES {
		RECT,
		CIRCLE
	};

	struct IntCircle {
		sf::Vector2i pos;
		int radius;
	};


public:
	struct Colliding {
		bool coll_top = false;
		bool coll_bot = false;
		bool coll_lef = false;
		bool coll_rig = false;
	};

	// RECTANGLE (by default it's a 0x0 rectangle) 
	Collision();
	Collision(sf::FloatRect r);
	Collision(int x, int y, int w, int h);

	// CIRCLE
	Collision(sf::Vector2i p, int r);
	Collision(int x, int y, int r);
	~Collision();

	bool Check_Collision(sf::FloatRect r);
	bool Check_Collision(IntCircle c);

	sf::Vector2f getCollisionSide(sf::FloatRect r, sf::Vector2f& oldVel);

	COLLISION_SHAPES type;
	sf::FloatRect rect;
	IntCircle circle;

	enum COLLISION_SIDE {
		NONE,
		N,
		E,
		S,
		W
	};

	COLLISION_SIDE collision_side = NONE;
};

