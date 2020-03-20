#pragma once
#include "GUItiledelement.h"

class GUIslider : public GUItiledelement {
	std::string id = "null";
	sf::Color leftcolor = sf::Color::Transparent;
	sf::Color rightcolor = sf::Color::Transparent;
	float maxvalue = 0;
	float minvalue = 0;

public:
	GUIslider() {}
	GUIslider(const GUIslider& copy) : GUItiledelement(copy) {
		id = copy.id;
		leftcolor = copy.leftcolor;
		rightcolor = copy.rightcolor;
	}

	void setMinMaxValue(float min, float max) { minvalue = min; maxvalue = max; }
	void setColor(sf::Color l, sf::Color r) { leftcolor = l; rightcolor = r; }

	void load() {
		GUItiledelement::load();
		if (!states.texture || leftcolor == sf::Color::Transparent || rightcolor == sf::Color::Transparent) {
			std::cout << "Slider texture not loaded\n";
			abort();
		}
		if(maxvalue <= minvalue) {
			std::cout << "Min/Max values are not valid\n";
			abort();
		}
		ninesliceInside();
		nineslice();
		alignElement();
	}

	void draw(sf::RenderWindow* w) override {
		GUItiledelement::draw(w);
	}

	void ninesliceInside() {
		sf::Vertex v;
		if (backgroundcolor != sf::Color::Transparent) v.color = backgroundcolor;
		sf::FloatRect boxrectnine = boxrect;
		boxrectnine.width += boxrect.left;
		boxrectnine.height += boxrect.top;

		float offset = states.texture->getSize().y/2;

		// LOAD CORNERS
		sf::Vector2f corner[4] = {
			sf::Vector2f(boxrectnine.left			 , boxrectnine.top),
			sf::Vector2f(boxrectnine.width - tilesize, boxrectnine.top),
			sf::Vector2f(boxrectnine.left			 , boxrectnine.height - tilesize),
			sf::Vector2f(boxrectnine.width - tilesize, boxrectnine.height - tilesize)
		};
		sf::Vector2f texCorner[4] = {
			sf::Vector2f(0			 , offset),
			sf::Vector2f(tilesize * 2, offset),
			sf::Vector2f(0			 , offset + tilesize * 2),
			sf::Vector2f(tilesize * 2, offset + tilesize * 2)
		};
		sf::Color colors[4] = {
			leftcolor, rightcolor, leftcolor, rightcolor
		};

		for (size_t i = 0; i < 4; i++) {
			v.position = corner[i];
			v.texCoords = texCorner[i];
			v.color = colors[i];
			appendQuad(v, sf::Vector2f(tilesize, tilesize));
		}

		v.position = sf::Vector2f(boxrectnine.left + tilesize, boxrectnine.top);
		v.color = leftcolor;
		vertexs.push_back(v);

		v.position = sf::Vector2f(boxrectnine.width - tilesize, boxrectnine.top);
		v.color = rightcolor;
		vertexs.push_back(v);

		v.position = sf::Vector2f(boxrectnine.width - tilesize, boxrectnine.height);
		v.color = rightcolor;
		vertexs.push_back(v);

		v.position = sf::Vector2f(boxrectnine.left + tilesize, boxrectnine.height);
		v.color = leftcolor;
		vertexs.push_back(v);
	}

	void changeControlState(CONTROL_STATES newcontrol) override {

	}
};
