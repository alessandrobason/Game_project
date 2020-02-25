#include <SFML/Graphics.hpp>
#include <iostream>
#include "Framework/RoomManager.h"

float min(float a, float b) { return (a < b ? a : b); }
float max(float a, float b) { return (a > b ? a : b); }
bool pressed = false;

void windowProcess(sf::RenderWindow* window, InputHandler *in) {
	sf::Event event;
	while (window->pollEvent(event)) {
		switch (event.type) {
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::Resized:
			//window->setView(sf::View(sf::FloatRect(0.f, 0.f, (float)event.size.width, (float)event.size.height)));
			break;
		case sf::Event::KeyPressed:
			in->setKeyDown(event.key.code);
			break;
		case sf::Event::KeyReleased:
			in->setKeyUp(event.key.code);
			break;
		case sf::Event::MouseMoved:
			in->setMousePosition(sf::Mouse::getPosition());
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left)
				in->setMouseLDown(true);
			if (event.mouseButton.button == sf::Mouse::Right)
				in->setMouseRDown(true);
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left)
				in->setMouseLDown(false);
			if (event.mouseButton.button == sf::Mouse::Right)
				in->setMouseRDown(false);
			break;
		default:
			// don't handle other events
			break;
		}
	}
}

int main() {
	sf::RenderWindow window(sf::VideoMode(16*12*5, 16*9*5), "Game Project");
	InputHandler *in = new InputHandler();

	RoomManager roomManager = RoomManager(&window, in);
	roomManager.loadMaps();

	sf::Clock deltaClock;

	roomManager.setDeltaTime(&deltaClock);
	// Game Loop
	while (window.isOpen()) {
		// Handle window events.
		windowProcess(&window, in);

		float dt = deltaClock.restart().asSeconds();
		roomManager.handleInput(dt);
		roomManager.update(dt);
		roomManager.draw();
	}
}

