#pragma once
#include "Room.h"
#include "GUI/GUImanager.h"

class MainScreen : public Room{
	
	GUImanager* guimanager = nullptr;
public:
	MainScreen() {}

	MainScreen(sf::RenderWindow* window, InputHandler* input, RoomManager* rm) : Room("MainScreen", window, input, rm) {
		guimanager = new GUImanager(window, input, rm);
	}

	void update(float dt) {
		main_camera = w->getDefaultView();
		w->setView(w->getDefaultView());
	}

	void draw() {
		guimanager->draw();
	}
};