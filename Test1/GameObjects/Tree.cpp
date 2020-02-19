#include "Tree.h"

Tree::Tree(sf::Texture* t, InputHandler* i, sf::Vector2f p) {
	in = i;
	txt = t;
	sprite.setTexture(*txt);
	sprite.setTextureRect(sf::IntRect(32, 96, 32, 64));
	sprite.setPosition(p);

	collider = Collision(p.x + 10, p.y + 42, 12, 12);
}

Tree::~Tree() {}

void Tree::draw(sf::RenderWindow* w) {
	w->draw(sprite);
}