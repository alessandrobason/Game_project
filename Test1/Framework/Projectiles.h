#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "../GameObjects/Enemy.h"

class Projectiles {
	struct singleprojectile {
		sf::Vector2f position;
		sf::Vector2f velocity;
		Collision collider;
		bool hitsomething = false;
	};

	float speed = 100.f;
	sf::Vector2f texture_coordinates;
	sf::VertexArray vertexs;
	sf::RenderStates states;
	// stores all the data of projectiles, its size
	// should be equal to projectils_vertexs' size / 4
	std::vector<singleprojectile> projectile_vector;

	std::vector<Collision*>* tilemapcollisions = nullptr;
	std::vector<Enemy*>* enemies = nullptr;
public:
	Projectiles() {}

	Projectiles(sf::Texture* txt, sf::Vector2i tex_coords) {
		vertexs.setPrimitiveType(sf::Quads);
		states.texture = txt;
		texture_coordinates = (sf::Vector2f) tex_coords;
	}

	void setEnemies(std::vector<Enemy*>* e) { enemies = e; }

	void update(float dt) {
		for (size_t i = 0; i < projectile_vector.size(); i++) {
			if (projectile_vector[i].hitsomething) continue; // TEMP
			sf::Vertex* v = &vertexs[i * 4];
			sf::Vector2f dt_velocity = projectile_vector[i].velocity * dt;
			std::cout << dt_velocity.x << " " << dt_velocity.y << "\n";
			v[0].position += dt_velocity;
			v[1].position += dt_velocity;
			v[2].position += dt_velocity;
			v[3].position += dt_velocity;
			projectile_vector[i].collider.moveCollision(dt_velocity);
			// check collisions
			for (size_t j = 0; j < enemies->size(); j++) {
				sf::FloatRect rect = enemies->at(j)->collider.rect;
				if (projectile_vector[i].collider.Check_Collision(rect)) {
					projectile_vector[i].hitsomething = true;
					enemies->at(j)->hit();
				}
			}
		}
	}

	void draw(sf::RenderWindow* w) {
		w->draw(vertexs, states);
	}

	void drawDebug(sf::RenderWindow* w) {
		for (size_t i = 0; i < projectile_vector.size(); i++) {
			projectile_vector[i].collider.drawDebug(w);
		}
	}

	void shoot(sf::Transform transform) {
		sf::Vertex vertex[4];
		vertex[0].position = transform.transformPoint(sf::Vector2f());
		vertex[1].position = transform.transformPoint(sf::Vector2f(16, 0));
		vertex[2].position = transform.transformPoint(sf::Vector2f(16, 16));
		vertex[3].position = transform.transformPoint(sf::Vector2f(0, 16));

		vertex[0].texCoords = texture_coordinates;
		vertex[1].texCoords = texture_coordinates + sf::Vector2f(16, 0);
		vertex[2].texCoords = texture_coordinates + sf::Vector2f(16, 16);
		vertex[3].texCoords = texture_coordinates + sf::Vector2f(0, 16);

		vertexs.append(vertex[0]);
		vertexs.append(vertex[1]);
		vertexs.append(vertex[2]);
		vertexs.append(vertex[3]);

		sf::Vector2f position = sf::Vector2f(transform.getMatrix()[12], transform.getMatrix()[13]);
		sf::Vector2f velocity = transform.transformPoint(sf::Vector2f(-speed, -speed));
		velocity -= position;
		sf::Vector2f centerofcollision = transform.transformPoint(sf::Vector2f(3, 3));
		Collision collider = Collision(centerofcollision.x - 4, centerofcollision.y - 4, 8, 8, Collision::PROJECTILE);
		projectile_vector.push_back({ position, velocity, collider });
	}

	void setSpeed(float s) { speed = s; }
};