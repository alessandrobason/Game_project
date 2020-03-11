#pragma once
#include "GUItiledelement.h"
#include "GUItext.h"

class GUImanager;

class GUIbutton : public GUItiledelement {
	sf::Texture* normal = nullptr;
	sf::Texture* hover = nullptr;
	sf::Texture* pressed = nullptr;
	
	GUItext insidetext = GUItext(this);

public:
	GUIbutton() : GUItiledelement() {}
	GUIbutton(const GUIbutton& copy);

	void setText(sf::Font* f, std::string text);
	void setTileSize(int t) { tilesize = t; }
	void setNormalTexture(sf::Texture* n) { normal = n; }
	void setHoverTexture(sf::Texture* h) { hover = h; }
	void setPressedTexture(sf::Texture* p) { pressed = p; }

	int getCharacterSize() { return insidetext.getCharacterSize(); }
	void setCharacterSize(int s) { insidetext.setCharacterSize(s); }

	void draw(sf::RenderWindow* w) override;

	virtual void trigger();

	void load();

	void changeControlState(CONTROL_STATES newcontrol) override;
};