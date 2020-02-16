#pragma once
#include <iostream>
#include "Framework/Room.h"
#include "Player.h"
#include "GameObjects/Tree.h"

class Circle_room : public Room {
public:
	Circle_room(sf::RenderWindow* window, InputHandler* input, std::string fold, std::string config_path);
	~Circle_room();

	void handleInput(float dt) override;
	void update(float dt) override;
	void draw() override;
	
private:
	Player *p;
};
