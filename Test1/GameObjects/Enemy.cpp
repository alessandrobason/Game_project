#include "Enemy.h"

Enemy::Enemy(sf::Texture* t, InputHandler* i, sf::RenderWindow* win, sf::Vector2f p) : GameObject(in, win) {
	txt = t;
	sprite.setTexture(*txt);
	sprite.setTextureRect(sf::IntRect(128, 112, 32, 32));
	sprite.setPosition(p);

	collider = Collision(p.x, p.y, 32, 32, Collision::OBJECT);
}

Enemy::~Enemy() {}

void Enemy::draw() {
	w->draw(sprite);
}