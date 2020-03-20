#pragma once
#include "Room.h"
#include "GUI/GUImanager.h"

class MainMenu : public Room, public GUImanager, public GUIcallback {
public:
	MainMenu() {}
	MainMenu(sf::RenderWindow* window, InputHandler* input, RoomManager* rm);

	void handleInput(float dt) {
		GUImanager::handleInput(dt);
	}

	void update(float dt);

	void draw() {
		GUImanager::draw();
	}

	void callback(std::string id, RESPONSE value) override;

	GUIbutton loadButton(std::string id);
};