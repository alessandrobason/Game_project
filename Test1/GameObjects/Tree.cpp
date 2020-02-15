#include "Tree.h"

Tree::Tree(sf::Texture* t, InputHandler* i, sf::Vector2f p) {
	in = i;
	txt = t;
	sprite.setTexture(*txt);
	sprite.setTextureRect(sf::IntRect(0, 48, 32, 64));
	sprite.setPosition(p);

	// metti a posto collisioni

	collisionShape.setPosition(sf::Vector2f(sprite.getPosition().x, sprite.getPosition().y+48));
	collisionShape.setSize(sf::Vector2f(32, 16));
	collisionShape.setOutlineColor(sf::Color::Red);
	collisionShape.setOutlineThickness(1.f);
	collisionShape.setFillColor(sf::Color::Transparent);
}

Tree::~Tree() {}

void Tree::draw(sf::RenderWindow* w) {
	w->draw(sprite);
}

void Tree::drawDebug(sf::RenderWindow* w) {
	w->draw(collisionShape);
}