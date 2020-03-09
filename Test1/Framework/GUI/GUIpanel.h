#pragma once
#include "GUIelement.h"

class GUIpanel {
	sf::Texture* backgroundimage = nullptr;
	std::vector<GUIelement> guielements;
public:

	void setBackground(sf::Texture* t) { backgroundimage = t; }
	
};