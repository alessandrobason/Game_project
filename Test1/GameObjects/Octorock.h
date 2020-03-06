#pragma once
#include "Enemy.h"

class Octorock : public Enemy {
public:
	Octorock(const Enemy& e);

	void update(float dt) override;
	void draw() override;

	void hit(float damage) override;
	bool animationCallback(std::string name) override;
};

