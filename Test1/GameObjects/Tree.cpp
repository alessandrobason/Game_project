#include "Tree.h"

Tree::Tree(sf::Texture* t, InputHandler* i, sf::RenderWindow* win, sf::Vector2f p) : GameObject(in, win) {
	txt = t;
	sprite.setTexture(*txt);
	sprite.setTextureRect(sf::IntRect(128, 112, 32, 32));
	sprite.setPosition(p);

	collider = Collision(p.x, p.y, 32, 32);
}

Tree::~Tree() {}

void Tree::draw() {
	w->draw(sprite);
}