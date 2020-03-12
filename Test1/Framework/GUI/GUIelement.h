#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <assert.h>

class GUIelement {
public:
	GUIelement() {}
	GUIelement(const GUIelement& copy);

	enum class CONTROL_STATES {
		NONE,
		HOVER,
		CLICK
	};

	enum class ALIGN {
		NONE,
		CENTER,
		LEFT,
		RIGHT,
		TOP,
		BOTTOM
	};

	int zbuffer = 0;
	bool enabled = true;
	GUIelement* oldpointer = nullptr;

	virtual void draw(sf::RenderWindow* w);

	virtual void trigger() {}
	virtual void load() {}
	virtual void changeControlState(CONTROL_STATES newcontrol) {}

	void setParent(GUIelement* p) { parent = p; zbuffer = parent->zbuffer + 1; }
	GUIelement* getParent() { return parent; }

	void setOffset(sf::Vector2f o) { elementoffset = o; }
	void setPosition(sf::Vector2f pos);
	void setCenter(sf::Vector2f p) { setPosition(sf::Vector2f(p.x - (boxrect.width / 2), p.y - (boxrect.height / 2))); }
	sf::Vector2f getPosition() { return sf::Vector2f(boxrect.left, boxrect.top); }
	sf::Vector2f getCenter() { return sf::Vector2f(boxrect.left, boxrect.top) + (sf::Vector2f(boxrect.width, boxrect.height) / 2.f); }
	sf::Vector2f getOffset() { return elementoffset; }

	void updatePosition(sf::Vector2f o = sf::Vector2f()) {
		setOffset(getOffset() + o);
		setPosition(getPosition());
	}

	virtual int getCharacterSize() { return 0; }
	virtual void setCharacterSize(int s) {}

	void setTexture(sf::Texture* t) { states.texture = t; }
	void setRect(sf::IntRect b) { boxrect = b; }

	void setAlign(ALIGN h, ALIGN v) { horizontal_alignment = h; vertical_alignment = v; }

	sf::IntRect getRect() { return boxrect; }
	CONTROL_STATES getState() { return currentcontrol; }


protected:
	GUIelement* parent = nullptr;

	std::vector<sf::Vertex> vertexs;
	sf::RenderStates states;
	sf::IntRect boxrect;
	sf::Vector2f elementoffset;
	CONTROL_STATES currentcontrol = CONTROL_STATES::NONE;
	ALIGN horizontal_alignment = ALIGN::NONE;
	ALIGN vertical_alignment = ALIGN::NONE;

	void alignElement();
};