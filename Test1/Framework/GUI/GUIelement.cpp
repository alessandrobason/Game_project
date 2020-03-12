#include "GUIelement.h"

GUIelement::GUIelement(const GUIelement& copy) {
	oldpointer = &const_cast<GUIelement&>(copy);
	parent = copy.parent;
	zbuffer = copy.zbuffer;
	enabled = copy.enabled;
	vertexs = copy.vertexs;
	states = copy.states;
	boxrect = copy.boxrect;
	currentcontrol = copy.currentcontrol;
	horizontal_alignment = copy.horizontal_alignment;
	vertical_alignment = copy.vertical_alignment;
}

void GUIelement::draw(sf::RenderWindow* w) {
	w->draw(vertexs.data(), vertexs.size(), sf::Quads, states);
}

void GUIelement::setPosition(sf::Vector2f pos) {
	pos += elementoffset;
	sf::Vector2f offset = pos - getPosition();
	states.transform.translate(offset);
	boxrect.left = pos.x;
	boxrect.top = pos.y;

	std::cout << "pos: " << offset.x << " " << offset.y << " " << elementoffset.x << " " << elementoffset.y << "\n";
}


void GUIelement::alignElement() {
	// HORIZONTAL
	switch (horizontal_alignment)
	{
	case ALIGN::CENTER:
		//std::cout << "parent center: " << parent->getCenter().x << " " << parent->getCenter().y << "\n";
		setCenter(sf::Vector2f(parent->getCenter().x, getCenter().y));
		break;
	case ALIGN::LEFT:
		setPosition(sf::Vector2f(parent->boxrect.left, boxrect.top));
		break;
	case ALIGN::RIGHT:
		setPosition(sf::Vector2f((parent->boxrect.left + parent->boxrect.width) - boxrect.width, boxrect.top));
		break;
	}
	// VERTICAL
	switch (vertical_alignment)
	{
	case ALIGN::CENTER:
		setCenter(sf::Vector2f(getCenter().x, parent->getCenter().y));
		break;
	case ALIGN::TOP:
		setPosition(sf::Vector2f(boxrect.left, parent->boxrect.top));
		break;
	case ALIGN::BOTTOM:
		setPosition(sf::Vector2f(boxrect.left, (parent->boxrect.top + parent->boxrect.height) - boxrect.height));
		break;
	}

	//setCenter(sf::Vector2f(192 / 2, 144 / 2));

	std::cout << "##---###" << getPosition().x << " " << getPosition().y << "\n";
}