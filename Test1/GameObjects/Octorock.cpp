#include "Octorock.h"

Octorock::Octorock(sf::Texture* t, InputHandler* i, sf::RenderWindow* win) : Enemy(t, i, win){

}

void Octorock::handleInput(float dt) {}
void Octorock::update(float dt) {
	Enemy::update(dt);
}
void Octorock::draw() {
	Enemy::draw();
}