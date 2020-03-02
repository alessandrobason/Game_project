#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "InputHandler.h"
#include "Collision.h"

/*
needs to have:
- sprite
- rotation pointer
*/

class Weapon : public sf::Sprite{
public:
	Weapon() {}
	Weapon(sf::Texture* txt, sf::Vector2i tex_coords, InputHandler* i);
	
	void handleInput(float dt);
	void update(float dt);
	void drawArrows(sf::RenderWindow* w);
	void drawDebug(sf::RenderWindow* w);

	void setAngle(float ang) { angle_to_mouse = ang; }
	void setCollisionArray(std::vector<Collision> c) {}

	void shoot();
	void move(sf::Vector2f offset);

protected:
	InputHandler* in = nullptr;
	float angle_to_mouse = 0.f;

	float projectile_speed = 100.f;
	
	struct projectile {
		sf::Vector2f position;
		sf::Vector2f velocity;
		Collision collider;
	};

	sf::Vector2f texture_coordinates;

	sf::VertexArray projectiles_vertexs;
	sf::RenderStates projectiles_states;
	// stores all the data of projectiles, its size
	// should be equal to projectils_vertexs' size / 4
	std::vector<projectile> projectile_vector;

};