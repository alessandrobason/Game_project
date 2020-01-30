#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Tilemap {
public:
	struct tilemap_data {
		int numb_layers;
		sf::Vector2u tileSize;
		int w;
		int h;
	};

	void setWindow(sf::RenderWindow* w);

	bool load(const std::string& tileset, std::vector<int> tiles, tilemap_data tilemapdata);

	void drawUnder();
	void drawOver();

	void setVertexArray(sf::VertexArray v);
	sf::VertexArray getVertexArray();

private:
	sf::VertexArray under;
	sf::VertexArray over;
	sf::Texture m_tileset;

	sf::RenderWindow* window;
	sf::RenderStates states;
};

