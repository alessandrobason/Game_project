#pragma once
#include "GUIelement.h"

class GUIpanel : public GUIelement {
	sf::Color backgroundcolor = sf::Color::Transparent;
public:

	void setBackgroundColor(sf::Color bc) { backgroundcolor = bc; }

	void load() {
		sf::Vertex v;
		v.position = sf::Vector2f(boxrect.left, boxrect.top);
		if (states.texture) {
			v.texCoords = sf::Vector2f();
		}
		appendQuad(v, sf::Vector2f(boxrect.width, boxrect.height));
	}

	void appendQuad(sf::Vertex v, sf::Vector2f size) {
		sf::Vector2f coords = v.position;
		sf::Vector2f texcoords = v.texCoords;
		sf::Vector2f positionoffset[4] = {
			sf::Vector2f(0, 0),
			sf::Vector2f(size.x, 0),
			sf::Vector2f(size.x, size.y),
			sf::Vector2f(0, size.y),
		};

		sf::Vector2f textureoffset[4] = { sf::Vector2f() };

		if (states.texture) {
			int w = states.texture->getSize().x;
			int h = states.texture->getSize().x;
			textureoffset[0] = sf::Vector2f(0, 0);
			textureoffset[1] = sf::Vector2f(w, 0);
			textureoffset[2] = sf::Vector2f(w, h);
			textureoffset[3] = sf::Vector2f(0, h);
		}

		for (size_t i = 0; i < 4; i++) {
			v.position = positionoffset[i] + coords;
			if (states.texture) v.texCoords = textureoffset[i] + texcoords;
			else v.color = backgroundcolor;
			vertexs.push_back(v);
		}
	}
};