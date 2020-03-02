#include "Weapon.h"

Weapon::Weapon(sf::Texture* txt, sf::Vector2i tex_coords, InputHandler* i) {
	in = i;
	setTexture(*txt);
	setTextureRect(sf::IntRect(tex_coords.x, tex_coords.y, 16, 16));
	setOrigin(12, 12);
	projectiles_vertexs.setPrimitiveType(sf::Quads);
	projectiles_states.texture = txt;
}

void Weapon::handleInput(float dt) {
	if (in->isMouseLPressed()) {
		shoot();
	}
}

void Weapon::update(float dt) {
	angle_to_mouse -= 45;
	setRotation(angle_to_mouse);
	std::cout << projectile_vector.size() << " " << projectiles_vertexs.getVertexCount() << "\n";
	for (size_t i = 0; i < projectile_vector.size(); i++) {
		sf::Vertex* v = &projectiles_vertexs[i * 4];
		sf::Vector2f dt_velocity = projectile_vector[i].velocity * dt;
		v[0].position += dt_velocity;
		v[1].position += dt_velocity;
		v[2].position += dt_velocity;
		v[3].position += dt_velocity;
		projectile_vector[i].collider.moveCollision(dt_velocity);
	}
	//std::cout << "angle: " << angle_to_mouse << "\n";
	//std::cout << "bow pos:\t" << getPosition().x << " " << getPosition().y << "\n";
}

void Weapon::drawArrows(sf::RenderWindow* w) {
	//w->draw(*this);
	w->draw(projectiles_vertexs, projectiles_states);
}

void Weapon::drawDebug(sf::RenderWindow* w) {
	for (size_t i = 0; i < projectile_vector.size(); i++) {
		projectile_vector[i].collider.drawDebug(w);
	}
}

void Weapon::shoot() {
	sf::Vertex vertex[4];
	//sf::Vertex* vertex = &projectiles_vertexs[projectiles_vertexs.getVertexCount()];
	sf::Transform weapontransform = getTransform();

	vertex[0].position = weapontransform.transformPoint(sf::Vector2f());
	vertex[1].position = weapontransform.transformPoint(sf::Vector2f(16, 0));
	vertex[2].position = weapontransform.transformPoint(sf::Vector2f(16, 16));
	vertex[3].position = weapontransform.transformPoint(sf::Vector2f(0, 16));
	
	sf::Vector2f texturecoordinates = sf::Vector2f(16, 0);
	vertex[0].texCoords = texturecoordinates;
	vertex[1].texCoords = texturecoordinates + sf::Vector2f(16, 0);
	vertex[2].texCoords = texturecoordinates + sf::Vector2f(16, 16);
	vertex[3].texCoords = texturecoordinates + sf::Vector2f(0, 16);

	projectiles_vertexs.append(vertex[0]);
	projectiles_vertexs.append(vertex[1]);
	projectiles_vertexs.append(vertex[2]);
	projectiles_vertexs.append(vertex[3]);

	sf::Vector2f position = sf::Vector2f(weapontransform.getMatrix()[12], weapontransform.getMatrix()[13]);
	sf::Vector2f velocity = weapontransform.transformPoint(sf::Vector2f(-projectile_speed, -projectile_speed));
	velocity -= position;
	sf::Vector2f centerofcollision = weapontransform.transformPoint(sf::Vector2f(3, 3));
	Collision collider = Collision(centerofcollision.x-4, centerofcollision.y-4, 8, 8);
	projectile_vector.push_back({ position, velocity, collider });
}

void Weapon::move(sf::Vector2f offset) {
	sf::Sprite::move(offset);
}