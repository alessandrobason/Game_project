#include "Player.h"

Player::Player() {
}

Player::Player(InputHandler* input, std::string txt_path, sf::Vector2f pos) {
	in = input; 

	if (!spritesheet.loadFromFile(txt_path)) {
		std::cout << "Failed to load " << txt_path << "\n";
	}
	sprite.setTexture(spritesheet);
	sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
	sprite.setPosition(pos);

	collision = Collision(0, 0, 16, 16);
	collisionShape.setPosition(sf::Vector2f(pos.x+1, pos.y+1));
	collisionShape.setSize(sf::Vector2f(collision.rect.width-2, collision.rect.height-2));
	collisionShape.setOutlineColor(sf::Color::Red);
	collisionShape.setOutlineThickness(1.f);
	collisionShape.setFillColor(sf::Color::Transparent);
}

void Player::handleInput(float dt) {
	oldVel = vel;
	vel = sf::Vector2f(0, 0);

	if (in->isKeyDown(in->KEY_UP)) {
		vel.y -= 1;
	}
	if (in->isKeyDown(in->KEY_DOWN)) {
		vel.y += 1;
	}
	if (in->isKeyDown(in->KEY_RIGHT)) {
		vel.x += 1;
	}
	if (in->isKeyDown(in->KEY_LEFT)) {
		vel.x -= 1;
	}

	float l = std::sqrt(vel.x * vel.x + vel.y * vel.y);
	if (l != 0)	vel /= l;
	vel *= speed;
}

void Player::update(float dt) {
	//std::cout << vel.x * dt << " " << vel.y * dt << "\n";
	move(vel * dt);
}

void Player::draw(sf::RenderWindow* w) {
	w->draw(sprite);
	w->draw(collisionShape);
}

void Player::move(sf::Vector2f mov) {
	sprite.move(mov);
	collisionShape.move(mov);
	collision.rect.left = sprite.getGlobalBounds().left;
	collision.rect.top = sprite.getGlobalBounds().top;
}
