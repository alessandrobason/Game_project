#pragma once
#include "Room.h"

class Circle_room : public Room {
public:
	Circle_room(sf::RenderWindow* window, std::string fold, std::string config_path);
	~Circle_room();

	void handle_input();
	void update();
	void draw();

	void ciao() const{
		std::cout << "ciao3\n";
	}
};
