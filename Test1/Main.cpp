#include <SFML/Graphics.hpp>
#include <iostream>
#include "Circle_room.h"

// https://www.sfml-dev.org/tutorials/2.5/graphics-view.php

float min(float a, float b) {return (a<b ? a:b);}

bool pressed = false;

void windowProcess(sf::RenderWindow* window, std::vector<Room*> rooms, int* current_room)
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
			rooms[*current_room]->handle_input();
			pressed = true;
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

	//Create the window

	std::vector<Room*> rooms;
	rooms.push_back(new Circle_room(&window, "Levels/Level2/", "Level2.json"));
	rooms.push_back(new Circle_room(&window, "Levels/Level1/", "Level1.json"));

	int current_room = 0;

	rooms[0]->load();
	std::cout << "-----------\n";
	rooms[1]->load();

	rooms[0]->ciao();

	// Game Loop
	while (window.isOpen())
	{
		// Handle window events.
		windowProcess(&window, rooms, &current_room);

		//render()


		rooms[current_room]->update();
		rooms[current_room]->draw();

		if (pressed) {
			current_room++;
			if (current_room == rooms.size()) current_room = 0;
			pressed = false;
		}

		// Call standard game loop functions (input, update and render)
		//level.handleInput();
		//level.update();
		//level.render();
	}
}