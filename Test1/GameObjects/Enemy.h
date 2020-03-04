#pragma once
#include "../Framework/GameObject.h"

class Enemy : public GameObject {
public:
	Enemy(sf::Texture* t, InputHandler* i, sf::RenderWindow* win, sf::Vector2f p);
	~Enemy();

	void draw() override;

	//float getY() override { return sprite.getPosition().y + 48; };

	float life = 50.f;

	void hit() override;

	sf::Sprite* getSprite() override { return &sprite; }

protected:
	sf::Sprite sprite;
};

