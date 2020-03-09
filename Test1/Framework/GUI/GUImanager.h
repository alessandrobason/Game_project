#pragma once
#include "../InputHandler.h"
#include "../RoomManager.fwd.h"
#include "GUIbox.h"

class GUImanager {
	InputHandler* in = nullptr;
	sf::RenderWindow* w = nullptr;
	RoomManager* roommanager = nullptr;

	GUIbox guibox;

public:
	GUImanager() {}
	GUImanager(sf::RenderWindow* window, InputHandler* input, RoomManager* rm);

	void handleInput(float dt) {

	}
	void update(float dt) {

	}
	void draw() {
		guibox.draw(w);
	}
};