#pragma once
#include "../Framework/GameObject.h"
#include "../Framework/Animation.h"

class Enemy : public GameObject {
public:
	Enemy() {}
	Enemy(const Enemy& e);
	Enemy(sf::Texture* t, InputHandler* i, sf::RenderWindow* win);
	~Enemy();

	void update(float dt) override;
	void draw() override;
	void hit() override;

	void setAnimatedSprite(AnimatedSprite a) { animation = a; }
	float getLife() { return life; }
	void setProjectile(bool p) { isprojectile = p; }

	sf::Sprite* getSprite() override { return animation.getSprite(); }

protected:
	//sf::Sprite sprite;
	AnimatedSprite animation;
	float life = 50.f;
	bool isprojectile = false;
};

