#include "Octorock.h"

Octorock::Octorock(const Enemy& e) : Enemy(e) {
	//animation.setCallback(&Octorock::animationCallback);
}

void Octorock::update(float dt) {
	Enemy::update(dt);
}

void Octorock::draw() {
	Enemy::draw();
}

void Octorock::hit(float damage) {
	std::cout << "life: " << life << " " << damage << "\n";
	life -= damage;
	if (life <= 0) {
		animation.setCurrentAnimation("death");
	}
}

bool Octorock::animationCallback(std::string name) {
	if (name == "death") {
		die();
		return true;
	}
	return false;
}