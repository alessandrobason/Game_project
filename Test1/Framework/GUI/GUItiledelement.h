#pragma once
#include "GUIelement.h"

class GUItiledelement : public GUIelement {
protected:
	int tilesize = NULL;
	GUItiledelement() : GUIelement() {}
	GUItiledelement(const GUItiledelement& copy) : GUIelement(copy) {}

	void load() override;
	void appendQuad(sf::Vertex v, sf::Vector2f size);
	void nineslice(int tilesize);
};