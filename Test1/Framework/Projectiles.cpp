#include "Projectiles.h"
#include "RoomManager.h"

Projectiles::Projectiles(sf::Texture* txt, sf::Vector2i tex_coords, Collision::LAYER l, RoomManager* rm) {
	//vertexs.setPrimitiveType(sf::Quads);
	states.texture = txt;
	texture_coordinates = (sf::Vector2f) tex_coords;
	collisionlayer = l;
	roommanager = rm;
}

void Projectiles::update(float dt) {
	for (size_t i = 0; i < projectile_vector.size(); i++) {
		if (projectile_vector[i].hitsomething) continue;
		sf::Vertex* v = &vertexs[i * 4];
		sf::Vector2f dt_velocity = projectile_vector[i].velocity * dt;
		v[0].position += dt_velocity;
		v[1].position += dt_velocity;
		v[2].position += dt_velocity;
		v[3].position += dt_velocity;
		projectile_vector[i].collider.moveCollision(dt_velocity);

		Collision* collision_hit = nullptr;

		// check collisions
		for (size_t j = 0; j < roommanager->getCurrentRoom()->getColliders()->size(); j++) {
			Collision* current_collision = roommanager->getCurrentRoom()->getColliders()->at(j);
			if (collisionlayer == current_collision->collisionlayer) continue;
			sf::FloatRect rect = current_collision->rect;
			if (projectile_vector[i].collider.Check_Collision(rect)) {
				collision_hit = current_collision;
				projectile_vector[i].hitsomething = true;
				removing_projectiles.push_back({ i, 1.f, 1.f, 255.f });
				break;
			}
		}

		// if the projectile hit something: check if it's a gameobject
		if (collision_hit == nullptr) continue;
		for (size_t j = 0; j < roommanager->getCurrentRoom()->getGameObjects()->size(); j++) {
			GameObject* current_gameobject = roommanager->getCurrentRoom()->getGameObjects()->at(j);
			if (&current_gameobject->collider == collision_hit) {
				//removing_projectiles.back().deathtime = 0.f;
				removing_projectiles.back().timeremaining = 0.f;
				current_gameobject->hit();
				break;
			}
		}
	}

	for (size_t i = 0; i < removing_projectiles.size(); i++) {
		std::cout << "alpha: " << removing_projectiles[i].alpha << "\n";
		removing_projectiles[i].timeremaining -= dt;
		float percentagepassed = 1 - (removing_projectiles[i].timeremaining / removing_projectiles[i].deathtime);
		removing_projectiles[i].alpha = Tween::lerp(255, 0, percentagepassed);
		size_t k = removing_projectiles[i].positioninarray * 4;
		for (size_t j = 0; j < 4; j++) {
			vertexs[k + j].color = sf::Color(255, 255, 255, removing_projectiles[i].alpha);
		}
		if (removing_projectiles[i].alpha <= 0 /*|| removing_projectiles[i].timeremaining <= 0*/) {
			removeArrow(removing_projectiles[i].positioninarray);
			i--;
		}
	}
}

void Projectiles::shoot(sf::Transform transform) {
	singleprojectile newprojectile;
	sf::Vertex vertex[4];

	vertex[0].position = transform.transformPoint(sf::Vector2f());
	vertex[1].position = transform.transformPoint(sf::Vector2f(16, 0));
	vertex[2].position = transform.transformPoint(sf::Vector2f(16, 16));
	vertex[3].position = transform.transformPoint(sf::Vector2f(0, 16));

	vertex[0].texCoords = texture_coordinates;
	vertex[1].texCoords = texture_coordinates + sf::Vector2f(16, 0);
	vertex[2].texCoords = texture_coordinates + sf::Vector2f(16, 16);
	vertex[3].texCoords = texture_coordinates + sf::Vector2f(0, 16);

	for (size_t i = 0; i < 4; i++) vertexs.push_back(vertex[i]);

	newprojectile.position = sf::Vector2f(transform.getMatrix()[12], transform.getMatrix()[13]);
	newprojectile.velocity = transform.transformPoint(sf::Vector2f(-speed, -speed));
	newprojectile.velocity -= newprojectile.position;
	sf::Vector2f centerofcollision = transform.transformPoint(sf::Vector2f(4, 4));
	newprojectile.collider = Collision(centerofcollision.x - 2, centerofcollision.y - 2, 4, 4, collisionlayer);
	projectile_vector.push_back(newprojectile);
}

void Projectiles::removeArrow(size_t n) {
	size_t vn = n * 4;
	vertexs.erase(vertexs.begin() + vn, vertexs.begin() + vn + 4);
	projectile_vector.erase(projectile_vector.begin() + n);
	for (size_t i = 0; i < removing_projectiles.size(); i++) {
		if (removing_projectiles[i].positioninarray == n) {
			removing_projectiles.erase(removing_projectiles.begin() + i);
			i--;
			continue;
		}
		if (removing_projectiles[i].positioninarray > n) removing_projectiles[i].positioninarray--;
	}
}