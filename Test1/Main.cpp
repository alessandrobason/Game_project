#include <SFML/Graphics.hpp>
#include <iostream>
#include "Circle_room.h"
#include "InputHandler.h"

// https://www.sfml-dev.org/tutorials/2.5/graphics-view.php

float min(float a, float b) {return (a<b ? a:b);}

bool pressed = false;

void windowProcess(sf::RenderWindow* window, InputHandler &in)
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::Resized:
			window->setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
			break;
		case sf::Event::KeyPressed:
			in.setKeyDown(event.key.code);
			break;
		default:
			// don't handle other events
			break;
		}
	}
}

int main()
{
	sf::RenderWindow window(sf::VideoMode(1000, 500), "CMP105_W1");
	InputHandler in;

	//Create the window

	std::vector<Room*> rooms;
	rooms.push_back(new Circle_room(&window, "Levels/Level2/", "Level2.json"));
	rooms.push_back(new Circle_room(&window, "Levels/Level1/", "Level1.json"));

	int current_room = 0;

	rooms[0]->load();
	rooms[1]->load();

	// Game Loop
	while (window.isOpen())
	{
		// Handle window events.
		windowProcess(&window, in);

		rooms[current_room]->handle_input();
		rooms[current_room]->update();
		rooms[current_room]->draw();

	}
}