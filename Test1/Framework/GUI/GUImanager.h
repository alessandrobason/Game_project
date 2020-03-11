#pragma once
#include "../InputHandler.h"
#include "../RoomManager.fwd.h"
#include "GUIbox.h"
#include "GUIbutton.h"

class GUImanager {
	InputHandler* in = nullptr;
	sf::RenderWindow* w = nullptr;
	RoomManager* roommanager = nullptr;

	//GUIbox guibox;
	std::vector<GUIelement*> elements;
	std::vector<sf::IntRect> elementsrects;

	struct textureload {
		std::string n;
		std::string f;
	};
	struct fontload {
		std::string n;
		std::string f;
	};

public:
	GUImanager() {}
	GUImanager(sf::RenderWindow* window, InputHandler* input, RoomManager* rm);

	void loadTextures(std::vector<textureload> texturenames);
	void loadFonts(std::vector<fontload> fontnames);

	void handleInput(float dt);
	void update(float dt);
	void draw();
};