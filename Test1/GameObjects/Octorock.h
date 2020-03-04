#pragma once
#include "Enemy.h"

class Octorock : public Enemy {
	//std::vector<sf::Vertex> vertexs;
public:
	Octorock(const Enemy& e) : Enemy(e) {}
	Octorock(sf::Texture* t, InputHandler* i, sf::RenderWindow* win);

	void handleInput(float dt) override;
	void update(float dt) override;
	void draw() override;
	//void hit() override;
};

