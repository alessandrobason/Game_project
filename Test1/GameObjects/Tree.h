#pragma once
#include "../Framework/GameObject.h"
#include "../Framework/Collision.h"

class Tree : public GameObject {
public:
	Tree(sf::Texture* t, InputHandler* i, sf::Vector2f p);
	~Tree();

	void draw(sf::RenderWindow* w) override;
	void drawDebug(sf::RenderWindow* w) override;

	float getY() override { return sprite.getPosition().y + 48; };

protected:
	sf::Sprite sprite;
	sf::RectangleShape collisionShape = sf::RectangleShape();
};

