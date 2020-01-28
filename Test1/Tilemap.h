#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Tilemap : public sf::Drawable, public sf::Transformable
{
public:
	struct tilemap_data {
		int numb_layers;
		sf::Vector2u tileSize;
		int w;
		int h;
	};

	bool load(const std::string& tileset, std::vector<int> tiles, tilemap_data tilemapdata);

private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::VertexArray m_vertices;
	sf::Texture m_tileset;
};

