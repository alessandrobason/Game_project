#include "Enemy.h"
#include "../Framework/RoomManager.h"

Enemy::Enemy(const Enemy& e) {
	w = e.w;
	in = e.in;
	roommanager = e.roommanager;
	//getSprite()->setPosition(sf::Vector2f(64, 700));
	collider = e.collider;
	animation = e.animation;
	speed = e.speed;
	life = e.life;
	animation.call_back_function = &GameObject::animationCallback;
	animation.setParent(this);
}

Enemy::~Enemy() {}

void Enemy::hit(float damage) {
	std::cout << "Enemy hit\n";
}

void Enemy::die() {
	roommanager->getCurrentRoom()->removeObject(this);
}

void Enemy::update(float dt) {
	vel = sf::Vector2f();

	if (in->isKeyPressed(sf::Keyboard::Num4)) {
		addTarget(getSprite()->getPosition() + sf::Vector2f(64, 0));
	}

	if (targets.size() > 0) {
		vel = targets[0] - getSprite()->getPosition();
		if (UsefulFunc::magnitude2(vel) <= 1.f) {
			targets.erase(targets.begin());
		}
		float l = UsefulFunc::InvSqrt(vel.x * vel.x + vel.y * vel.y);
		vel *= l;
		vel *= speed;
	}

	animation.animate(dt);
	move(vel * dt);
}

void Enemy::draw() {
	animation.draw(w);
	//w->draw(sprite);
}