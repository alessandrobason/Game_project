#pragma once
#include <iostream>
#include "Room.h"
#include "Player.h"

class Circle_room : public Room {
public:
	Circle_room(sf::RenderWindow* window, InputHandler* input, std::string fold, std::string config_path);
	~Circle_room();

	void handle_input();
	void update(float dt);
	void draw();
private:
	Player *p;
};
