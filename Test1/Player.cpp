#include "Player.h"

void Player::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	states.texture = &m_tileset;

	target.draw(m_vertices, states);
}