#pragma once
#include <stdlib.h>
#include "../Framework/GameObject.h"
#include "../Framework/Animation.h"

class Map_room;

class Enemy : public GameObject {
public:
	Enemy();
	Enemy(const Enemy& e);
	~Enemy();

	void update(float dt) override;
	void draw() override;
	void drawDebug() override;
	void hit(float damage) override;
	void move(sf::Vector2f mov) override;
	void setPosition(sf::Vector2f position) override;

	void die();

	void setAnimatedSprite(AnimatedSprite a) { animation = a; }
	void setLife(float l) { life = l; }
	float getLife() { return life; }
	void setProjectile(bool p) { isprojectile = p; }

	void isPlayerInside(bool p) { isplayerinside = p; }

	sf::Sprite* getSprite() override { return animation.getSprite(); }

protected:
	enum DIRECTION {
		UP, DOWN, LEFT, RIGHT
	};
	
	std::string directionnames[4] = { "up", "down", "left", "right" };
	AnimatedSprite animation;
	DIRECTION last_direction = DIRECTION::DOWN;
	float life = 50.f;
	bool isprojectile = false;
	sf::Vector2f target;

	float angletoplayer = 0.f;
	bool isplayerinside = false;
	bool moving = false;
	Map_room *roompointer = nullptr;

	//FINITE STATE MACHINE (AI)
	enum class states {
		IDLE,
		ATTACK,
		MOVING
	} currentstate;
};

