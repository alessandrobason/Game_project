#pragma once
#include "GUIelement.h"

class GUIbox : public GUIelement {
	std::vector<sf::Vertex> vertexs;
	sf::RenderStates states;
	sf::IntRect boxrect;
	float scale = 1.f;

public:

	void draw(sf::RenderWindow* w) {
		w->draw(vertexs.data(), vertexs.size(), sf::Quads, states);
	}

	void setTexture(sf::Texture* t) { states.texture = t; }
	void setRect(sf::IntRect b) { boxrect = b; }

	void load() {
		if (boxrect.width < 32 || boxrect.height < 32) {
			assert("boxrect is too small");
		}
		//boxrect.width *= scale;
		//boxrect.height *= scale;

		// LOAD CORNERS
		sf::Vector2f corner[4] = {
			sf::Vector2f(boxrect.left	   , boxrect.top),
			sf::Vector2f(boxrect.width - 16, boxrect.top),
			sf::Vector2f(boxrect.left	   , boxrect.height - 16),
			sf::Vector2f(boxrect.width - 16, boxrect.height - 16)
		};
		sf::Vector2f texCorner[4] = {
			sf::Vector2f(0, 0),
			sf::Vector2f(32, 0),
			sf::Vector2f(0, 32),
			sf::Vector2f(32, 32)
		};
		sf::Vector2f coords[4] = {
			sf::Vector2f(0, 0),
			sf::Vector2f(16, 0),
			sf::Vector2f(16, 16),
			sf::Vector2f(0, 16),
		};
		float size = 16 * scale;

		for (size_t i = 0; i < 4; i++) {
			for (size_t j = 0; j < 4; j++) {
				sf::Vertex v;
				v.position = corner[i] + (coords[j] * scale);
				v.texCoords = texCorner[i] + coords[j];
				vertexs.push_back(v);
			}
		}

		// LOAD INBETWEENS
		sf::IntRect inbCorners[4] = {
			sf::IntRect(sf::Vector2i(boxrect.left + 16, boxrect.top), sf::Vector2i(boxrect.width - 32, 16)), // top
			sf::IntRect(sf::Vector2i(boxrect.left, boxrect.top + 16), sf::Vector2i(16, boxrect.height - 32)), // left
			sf::IntRect(sf::Vector2i(boxrect.width - 16, boxrect.top + 16), sf::Vector2i(16, boxrect.height - 32)), // right
			sf::IntRect(sf::Vector2i(boxrect.left + 16, boxrect.height - 16), sf::Vector2i(boxrect.width - 32, 16)) // bottom
		};

		sf::Vector2f inbTexCorner[4] = {
			sf::Vector2f(16, 0),
			sf::Vector2f(0, 16),
			sf::Vector2f(32, 16),
			sf::Vector2f(16, 32)
		};

		for (size_t i = 0; i < 4; i++) {
			sf::Vertex v[4];
			v[0].position = sf::Vector2f(inbCorners[i].left, inbCorners[i].top);
			v[1].position = sf::Vector2f(inbCorners[i].left + inbCorners[i].width, inbCorners[i].top);
			v[2].position = sf::Vector2f(inbCorners[i].left + inbCorners[i].width, inbCorners[i].top + inbCorners[i].height);
			v[3].position = sf::Vector2f(inbCorners[i].left, inbCorners[i].top + inbCorners[i].height);
			for (size_t j = 0; j < 4; j++) {
				v[j].texCoords = inbTexCorner[i] + coords[j];
				vertexs.push_back(v[j]);
			}
		}
	}
};