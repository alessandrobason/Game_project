#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "InputHandler.h"
#include "../GameObjects/Enemy.h"
#include "Projectiles.h"

/*
needs to have:
- sprite
- rotation pointer
*/

class Weapon : public sf::Sprite{
public:
	Weapon() {}
	Weapon(sf::Texture* txt, sf::Vector2i tex_coords, InputHandler* i, Collision::LAYER l, RoomManager* rm);
	
	void handleInput(float dt);
	void update(float dt);
	void drawArrows(sf::RenderWindow* w);
	void drawDebug(sf::RenderWindow* w);

	void setGameObjects(std::vector<GameObject*>* g) { projectiles.setGameObjects(g); }
	void setTilemap(std::vector<Collision*>* t) { projectiles.setTilemap(t); }
	void setAngle(float ang) { angle_to_mouse = ang; }
	void setCollisionArray(std::vector<Collision*>* c) {}

	void shoot();
	void move(sf::Vector2f offset);

protected:
	InputHandler* in = nullptr;
	float angle_to_mouse = 0.f;
	
	sf::Vector2f texture_coordinates;

	Projectiles projectiles;
};