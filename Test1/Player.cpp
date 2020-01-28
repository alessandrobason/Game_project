#include "Player.h"

Player::Player() { std::cout << "bitch\n"; }

Player::Player(InputHandler* input, std::string txt_path, sf::Vector2f pos) {
	in = input; 

	if (!spritesheet.loadFromFile(txt_path)) {
		std::cout << "Failed to load " << txt_path << "\n";
	}
	sprite.setTexture(spritesheet);
	sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
}

void Player::Update(float dt) {
	sf::Vector2f vel(0, 0);

	if (in->isKeyDown(in->KEY_UP)) {
		vel.y -= speed * 0.01f;
		//move(sf::Vector2f(0, -speed) * 0.01f);
	}
	if (in->isKeyDown(in->KEY_DOWN)) {
		vel.y += speed * 0.01f;
		//move(sf::Vector2f(0, speed) * 0.01f);
	}
	if (in->isKeyDown(in->KEY_RIGHT)) {
		vel.x += speed * 0.01f;
		//move(sf::Vector2f(speed, 0) * 0.01f);
	}
	if (in->isKeyDown(in->KEY_LEFT)) {
		vel.x -= speed * 0.01f;
		//move(sf::Vector2f(-speed, 0) * 0.01f);
	}
	move(vel);
}

void Player::move(sf::Vector2f mov) {
	sprite.move(mov);
}
