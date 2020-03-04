#pragma once
#include <SFML/Graphics.hpp>
#define _USE_MATH_DEFINES
#include <math.h>

class Collision {
private:
	bool isenabled = true;

	sf::RectangleShape debug;

	void setupDebugBox(sf::FloatRect r);

	enum COLLISION_SHAPES {
		RECT,
		CIRCLE
	};

	struct IntCircle {
		sf::Vector2i pos;
		int radius;
	};

public:
	enum class LAYER {
		ALL,
		OBJECT,
		PLAYER
	};
	
	// RECTANGLE (by default it's a 0x0 rectangle) 
	Collision();
	Collision(sf::FloatRect r, LAYER type);
	Collision(int x, int y, int w, int h, LAYER type);

	// CIRCLE
	Collision(sf::Vector2i p, int r);
	Collision(int x, int y, int r);
	~Collision();

	void setEnabled(bool e) { isenabled = e; }
	void enable() { isenabled = true; }
	void disable() { isenabled = false; }
	bool isEnabled() { return isenabled; }

	void setDebugPosition(sf::Vector2f pos) { debug.setPosition(pos); }
	void setDebugSize(sf::Vector2f size) { debug.setSize(size); }
	void setDebugColor(sf::Color c) { debug.setOutlineColor(c); }

	bool Check_Collision(sf::FloatRect r);
	bool Check_Collision(IntCircle c);

	void moveCollision(sf::Vector2f mov);
	
	sf::Vector2f getCollisionSide(sf::FloatRect r, sf::Vector2f& oldVel);

	void drawDebug(sf::RenderWindow* w);

	COLLISION_SHAPES type;
	LAYER collisionlayer;
	sf::FloatRect rect;
	sf::Vector2f collision_offset;
	IntCircle circle;
};