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

	void handleInput(float dt) {
		guimanager->handleInput(dt);
	}

	void update(float dt) {
		//main_camera = w->getDefaultView();
		//w->setView(w->getDefaultView());
		main_camera.setViewport(in->getView().getViewport());
		w->setView(main_camera);
		//sf::Vector2f cameratopleft = main_camera.getCenter() - main_camera.getSize() / 2.f;
		//std::cout << cameratopleft.x << " " << cameratopleft.y << "\r";
		guimanager->update(dt);
	}

	void draw() {
		guimanager->draw();
	}
};