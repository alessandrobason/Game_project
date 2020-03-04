#include "Enemy.h"

Enemy::Enemy(const Enemy& e) {
	//Enemy::Enemy(e.txt, e.in, e.w);
	getSprite()->setPosition(sf::Vector2f(64, 700));
	//sprite.setPosition(e.sprite.getPosition());
	collider = e.collider;
	animation = e.animation;
}

Enemy::Enemy(sf::Texture* t, InputHandler* i, sf::RenderWindow* win) : GameObject(in, win) {
	txt = t;
	//sprite.setTexture(*txt);
	//sprite.setTextureRect(sf::IntRect(128, 112, 32, 32));
	//sprite.setPosition(p);
	collisionlayer = Collision::LAYER::ENEMY;
	//collider = Collision(p.x, p.y, 32, 32, collisionlayer);
}

Enemy::~Enemy() {}

void Enemy::hit() {
	//GameObject::hit();
	std::cout << "Enemy hit\n";
	life -= 10;
}

void Enemy::update(float dt) {
	animation.animate(dt);
	std::cout << "sono qui\n";
}

void Enemy::draw() {
	animation.draw(w);
	//w->draw(sprite);
}