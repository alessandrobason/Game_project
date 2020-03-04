#include "Enemy.h"

Enemy::Enemy(sf::Texture* t, InputHandler* i, sf::RenderWindow* win, sf::Vector2f p) : GameObject(in, win) {
	txt = t;
	sprite.setTexture(*txt);
	sprite.setTextureRect(sf::IntRect(128, 112, 32, 32));
	sprite.setPosition(p);
	collisionlayer = Collision::LAYER::OBJECT;
	collider = Collision(p.x, p.y, 32, 32, collisionlayer);
}

Enemy::~Enemy() {}

void Enemy::hit() {
	//GameObject::hit();
	std::cout << "Enemy hit\n";
	life -= 10;
}

void Enemy::draw() {
	w->draw(sprite);
}