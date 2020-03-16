#include "Enemy.h"
#include "../Framework/RoomManager.h"

Enemy::Enemy() {
	checkboxsize = sf::Vector2f(200, 200);
	checkbox = Collision(-checkboxsize.x / 2.f, -checkboxsize.y / 2.f, checkboxsize.x / 2.f, checkboxsize.y / 2.f, Collision::LAYER::ENEMY);
	checkbox.setDebugColor(sf::Color::Magenta);
}

Enemy::Enemy(const Enemy& e) {
	w = e.w;
	in = e.in;
	roommanager = e.roommanager;
	collider = e.collider;
	animation = e.animation;
	speed = e.speed;
	life = e.life;
	checkboxsize = e.checkboxsize;
	checkbox = e.checkbox;
	animation.call_back_function = &GameObject::animationCallback;
	animation.setParent(this);

	roompointer = roommanager->getCurrentRoom();
}

Enemy::~Enemy() {}

void Enemy::hit(float damage) {
	std::cout << "Enemy hit\n";
}

void Enemy::move(sf::Vector2f mov) {
	GameObject::move(mov);
	checkbox.moveCollision(mov);
}

void Enemy::setPosition(sf::Vector2f position) {
	GameObject::setPosition(position);
	position.x += collider.rect.width / 2;
	position.y += collider.rect.height / 2;
	checkbox.setCenter(position);
}

void Enemy::die() {
	roommanager->getCurrentRoom()->removeObject(this);
}

void Enemy::update(float dt) {
	animation.animate(dt);
	if (dead) return;
	vel = sf::Vector2f();

	std::string anim;

	switch (currentstate) {
	case states::IDLE:
		if(collided) target = sf::Vector2f();
	case states::MOVING:
		vel = target - getCenter();
		if (UsefulFunc::magnitude2(vel) <= 1.f) target = sf::Vector2f();
		vel = UsefulFunc::normalize(vel);
		if (vel.x <= -0.7)		last_direction = LEFT;
		else if (vel.x >= 0.7)	last_direction = RIGHT;
		else if (vel.y <= -0.7) last_direction = UP;
		else					last_direction = DOWN;
		break;
	case states::ATTACK:
		sf::Vector2f p = roompointer->getPlayer()->getCenter() - getCenter();
		angletoplayer = (UsefulFunc::atan2(p.y, p.x) * 180 / M_PI + 180);
		vel = UsefulFunc::normalize(p);
		if (vel.x <= -0.7)		last_direction = LEFT;
		else if (vel.x >= 0.7)	last_direction = RIGHT;
		else if (vel.y <= -0.7) last_direction = UP;
		else					last_direction = DOWN;
		if (UsefulFunc::magnitude2(p) <= 576 /*24^2*/) vel = sf::Vector2f();
		break;
	}

	anim = "idle ";
	if(vel != sf::Vector2f()) anim = "walk ";


	anim += directionnames[last_direction];
	animation.setCurrentAnimation(anim);

	move(vel * speed * dt);
	collided = false;
}

void Enemy::draw() {
	animation.draw(w);
}

void Enemy::drawDebug() {
	GameObject::drawDebug();
	checkbox.drawDebug(w);
}