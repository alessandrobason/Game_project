#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tween.h"
#include "../GameObjects/Enemy.h"
#include "RoomManager.fwd.h"

class Projectiles {
	struct singleprojectile {
		sf::Vector2f position;
		sf::Vector2f velocity;
		Collision collider;
		bool hitsomething = false;
	};

	struct projectiledeath {
		size_t positioninarray;
		float deathtime = 0.f;
		float timeremaining = 0.f;
		float alpha = 255.f;
	};

	float speed = 100.f;
	sf::Vector2f texture_coordinates;
	std::vector<sf::Vertex> vertexs;
	//sf::VertexArray vertexs;
	sf::RenderStates states;
	// stores all the data of projectiles, its size
	// should be equal to projectils_vertexs' size / 4
	std::vector<singleprojectile> projectile_vector;
	std::vector<projectiledeath> removing_projectiles;

	Collision::LAYER collisionlayer = Collision::LAYER::ALL;

	RoomManager* roommanager = nullptr;
	std::vector<Collision*>* tilemapcollisions = nullptr;
	std::vector<GameObject*>* gameobjects = nullptr;
public:
	Projectiles() {}

	Projectiles(sf::Texture* txt, sf::Vector2i tex_coords, Collision::LAYER l, RoomManager* rm);

	void setGameObjects(std::vector<GameObject*>* g) { gameobjects = g; }
	void setTilemap(std::vector<Collision*>* t) { tilemapcollisions = t; }

	void update(float dt);

	void draw(sf::RenderWindow* w) {
		//w->draw(vertexs, states);
		w->draw(vertexs.data(), vertexs.size(), sf::Quads, states);
	}

	void drawDebug(sf::RenderWindow* w) {
		for (size_t i = 0; i < projectile_vector.size(); i++) {
			projectile_vector[i].collider.drawDebug(w);
		}
	}

	void shoot(sf::Transform transform);
	void removeArrow(size_t n);

	void setSpeed(float s) { speed = s; }
};