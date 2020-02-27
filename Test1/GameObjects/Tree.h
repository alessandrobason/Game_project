#pragma once
#include "../Framework/GameObject.h"
#include "../Framework/Collision.h"

class Tree : public GameObject {
public:
	Tree(sf::Texture* t, InputHandler* i, sf::RenderWindow* win, sf::Vector2f p);
	~Tree();

	void draw() override;

	//float getY() override { return sprite.getPosition().y + 48; };

	sf::Sprite* getSprite() override { return &sprite; }

protected:
	sf::Sprite sprite;
};

