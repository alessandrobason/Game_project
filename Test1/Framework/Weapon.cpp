#include "Weapon.h"

Weapon::Weapon(sf::Texture* txt, sf::Vector2i tex_coords, InputHandler* i, Collision::LAYER l, RoomManager* rm) {
	in = i;
	setTexture(*txt);
	setTextureRect(sf::IntRect(tex_coords.x, tex_coords.y, 16, 16));
	setOrigin(12, 12);
	projectiles = Projectiles(txt, sf::Vector2i(16, 0), l, rm);
}

void Weapon::handleInput(float dt) {
	if (in->isMouseLPressed()) {
		shoot();
	}
}

void Weapon::update(float dt) {
	angle_to_mouse -= 45;
	setRotation(angle_to_mouse);
	projectiles.update(dt);
}

void Weapon::drawArrows(sf::RenderWindow* w) {
	projectiles.draw(w);
}

void Weapon::drawDebug(sf::RenderWindow* w) {
	projectiles.drawDebug(w);
}

void Weapon::shoot() {
	projectiles.shoot(getTransform());
}

void Weapon::move(sf::Vector2f offset) {
	sf::Sprite::move(offset);
}