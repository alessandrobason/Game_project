#pragma once
#include "Room.h"
#include "GUI/GUImanager.h"
#include "GUI/GUIsplashscreen.h"

class SplashScreens : public Room, public GUImanager {
	bool finishedanimation = false;

public:
	SplashScreens() {}

	SplashScreens(sf::RenderWindow* window, InputHandler* input, RoomManager* rm);

	void handleInput(float dt) override {
		GUImanager::handleInput(dt);
	}

	void update(float dt) override;

	void draw() {
		GUImanager::draw();
	}
};