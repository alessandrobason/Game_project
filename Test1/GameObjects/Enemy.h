#pragma once
#include "../Framework/GameObject.h"
#include "../Framework/Animation.h"

class Enemy : public GameObject {
public:
	Enemy() {}
	Enemy(const Enemy& e);
	~Enemy();

	void update(float dt) override;
	void draw() override;
	void hit(float damage) override;

	void die();

	void setAnimatedSprite(AnimatedSprite a) { animation = a; }
	void setLife(float l) { life = l; }
	float getLife() { return life; }
	void setProjectile(bool p) { isprojectile = p; }
	void addTarget(sf::Vector2f t) { targets.push_back(t); }

	sf::Sprite* getSprite() override { return animation.getSprite(); }

protected:
	AnimatedSprite animation;
	float life = 50.f;
	bool isprojectile = false;
	std::vector<sf::Vector2f> targets;
};

