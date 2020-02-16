#include "Tree.h"

Tree::Tree(sf::Texture* t, InputHandler* i, sf::Vector2f p) {
	in = i;
	txt = t;
	sprite.setTexture(*txt);
	sprite.setTextureRect(sf::IntRect(0, 48, 32, 64));
	sprite.setPosition(p);

	collider = Collision(p.x, p.y + 48, 32, 16);
}

Tree::~Tree() {}

void Tree::draw(sf::RenderWindow* w) {
	w->draw(sprite);
}