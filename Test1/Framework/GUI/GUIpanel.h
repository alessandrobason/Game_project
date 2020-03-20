#pragma once
#include "GUIelement.h"

class GUIpanel : public GUIelement {
public:


	void load() {
		sf::Vertex v;
		v.position = sf::Vector2f(boxrect.left, boxrect.top);
		if (states.texture) {
			v.texCoords = sf::Vector2f();
		}
		appendQuad(v, sf::Vector2f(boxrect.width, boxrect.height));
	}
};