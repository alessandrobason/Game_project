#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "Collision.h"

class Tilemap {
public:
	struct tilemap_data {
		int numb_layers;
		sf::Vector2u tileSize;
		int w;
		int h;
	};

	void setWindow(sf::RenderWindow* w);

	void setTexture(sf::Texture* txt) { m_tileset = txt; }
	sf::Texture* getTexture() { return m_tileset; }

	bool load(bool* d, sf::Texture* t, std::vector<int> tiles, tilemap_data tilemapdata);
	bool loadCollisions(std::vector<Collision> collisions);

	void drawUnder();
	void drawOver();
	void setShader(sf::Shader s);

	void setVertexArray(sf::VertexArray v);
	sf::VertexArray getVertexArray();

	std::vector<Collision> collisions;
	std::vector<sf::RectangleShape> collisionShapes;

private:
	sf::VertexArray under;
	sf::VertexArray over;
	sf::Texture* m_tileset;

	sf::RenderWindow* window;
	sf::RenderStates states;

	bool* isdebug = nullptr;
};

