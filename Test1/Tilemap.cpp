#include "Tilemap.h"

bool Tilemap::load(const std::string& tileset, std::vector<int> tiles, tilemap_data tilemapdata) {

	if (!m_tileset.loadFromFile(tileset)) {
		std::cout << "Failed to load tileset\n";
		return false;
	}

	// set the primitive of the vertex array to quads (instead of triangles)
	under.setPrimitiveType(sf::Quads);
	over.setPrimitiveType(sf::Quads);

	// set the size of the vertex array to be enough for the whole tilemap
	const int layer_dimension = tiles.size() / tilemapdata.numb_layers;
	
	under.resize(layer_dimension * (tilemapdata.numb_layers-1) * 4);
	over.resize(layer_dimension * 4);

	for (int nl = 0; nl < tilemapdata.numb_layers; nl++) {
		for (int y = 0; y < tilemapdata.h; y++) {
			for (int x = 0; x < tilemapdata.w; x++) {
				int position = (nl * layer_dimension) + (y * tilemapdata.w) + (x);
				
				// get tile
				int tileNumber = tiles[position];

				// if tile is empty
				if (tileNumber == -1) continue;

				// get position in texture
				int tu = tileNumber % (m_tileset.getSize().x / tilemapdata.tileSize.x);
				int tv = tileNumber / (m_tileset.getSize().x / tilemapdata.tileSize.x);

				// pointer to the tile's quad
				sf::Vertex* quad = &under[position * 4];

				if(nl == tilemapdata.numb_layers-1){
					position = (y * tilemapdata.w) + (x);
					quad = &over[position * 4];
				}

				// set position of 4 vertices (cw order)
				quad[0].position = sf::Vector2f(x * tilemapdata.tileSize.x,			y * tilemapdata.tileSize.y);
				quad[1].position = sf::Vector2f((x + 1) * tilemapdata.tileSize.x,	y * tilemapdata.tileSize.y);
				quad[2].position = sf::Vector2f((x + 1) * tilemapdata.tileSize.x,	(y + 1) * tilemapdata.tileSize.y);
				quad[3].position = sf::Vector2f(x * tilemapdata.tileSize.x,			(y + 1) * tilemapdata.tileSize.y);

				// set texture coords (cw order)
				quad[0].texCoords = sf::Vector2f(tu * tilemapdata.tileSize.x,		tv * tilemapdata.tileSize.y);
				quad[1].texCoords = sf::Vector2f((tu + 1) * tilemapdata.tileSize.x, tv * tilemapdata.tileSize.y);
				quad[2].texCoords = sf::Vector2f((tu + 1) * tilemapdata.tileSize.x, (tv + 1) * tilemapdata.tileSize.y);
				quad[3].texCoords = sf::Vector2f(tu * tilemapdata.tileSize.x,		(tv + 1) * tilemapdata.tileSize.y);
			}
		}
	}
	std::cout << "Mv:" << under.getVertexCount() << "\n";
	std::cout << "Mv:" << over.getVertexCount() << "\n";

	states.texture = &m_tileset;
	
	return true;
}

void Tilemap::setWindow(sf::RenderWindow* w){
	window = w;
}
/*

void Tilemap::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	states.texture = &m_tileset;

	target.draw(under, states);
	target.draw(over, states);
}
*/

void Tilemap::drawUnder(){
	window->draw(under, states);
}

void Tilemap::drawOver(){
	window->draw(over, states);
}

void Tilemap::setVertexArray(sf::VertexArray v) {
	under = v;
}
sf::VertexArray Tilemap::getVertexArray() {
	return under;
}