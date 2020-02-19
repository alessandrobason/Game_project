#include <SFML/Graphics.hpp>
#include <iostream>
#include "Forest_room.h"

/*

void windowProcess(sf::RenderWindow* window, InputHandler* in);

float min(float a, float b) { return (a < b ? a : b); }
float max(float a, float b) { return (a > b ? a : b); }

struct gameobjectsorting {
	inline bool operator() (sf::Vertex& g1, sf::Vertex& g2) {
		return g1.position.y < g2.position.y;
	}
};

int main() {
	InputHandler* in = new InputHandler();

	sf::Image playerSprite;
	sf::Image tileset;
	sf::Image allImages;

	sf::Vector2f playerOffset;

	if (!playerSprite.loadFromFile("Assets/main character.png")) std::cout << "player\n";
	if (!tileset.loadFromFile("Assets/tileset.png")) std::cout << "tileset\n";
	
	int x = playerSprite.getSize().x + tileset.getSize().x;
	int y = max(playerSprite.getSize().y, tileset.getSize().y);

	allImages.create(x, y);
	allImages.copy(tileset, 0, 0);
	allImages.copy(playerSprite, tileset.getSize().x, 0);

	playerOffset.x = tileset.getSize().x;
	playerOffset.y = 0;

	sf::Texture finalTxt;
	finalTxt.loadFromImage(allImages);

	sf::VertexArray objects;
	objects.setPrimitiveType(sf::Quads);

	sf::VertexArray arr;
	arr.setPrimitiveType(sf::Quads);

	sf::Vector2i coord = sf::Vector2i(0, 0);

	sf::Vector2f text[] {
		sf::Vector2f(0, 0), sf::Vector2f(16, 0), sf::Vector2f(16, 0), sf::Vector2f(16, 0), sf::Vector2f(32, 0),
		sf::Vector2f(0, 16), sf::Vector2f(16, 16), sf::Vector2f(16, 16), sf::Vector2f(16, 16), sf::Vector2f(32, 16),
		sf::Vector2f(0, 16), sf::Vector2f(16, 16), sf::Vector2f(16, 16), sf::Vector2f(16, 16), sf::Vector2f(32, 16),
		sf::Vector2f(0, 16), sf::Vector2f(16, 16), sf::Vector2f(16, 16), sf::Vector2f(16, 16), sf::Vector2f(32, 16),
		sf::Vector2f(0, 32), sf::Vector2f(16, 32), sf::Vector2f(16, 32), sf::Vector2f(16, 32), sf::Vector2f(32, 32),
	};

	for (size_t i = 0; i < 25; i++) {
		sf::Vertex quad[4];

		int x = (i % 5) * 16;
		int y = (i / 5) * 16;

		sf::Vector2f pos = sf::Vector2f(x, y);

		quad[0].position = pos;
		quad[1].position = pos + sf::Vector2f(16, 0);
		quad[2].position = pos + sf::Vector2f(16, 16);
		quad[3].position = pos + sf::Vector2f(0, 16);

		quad[0].texCoords = text[i];
		quad[1].texCoords = text[i] +  sf::Vector2f(16, 0);
		quad[2].texCoords = text[i] + sf::Vector2f(16, 16);
		quad[3].texCoords = text[i] + sf::Vector2f(0, 16);

		arr.append(quad[0]);
		arr.append(quad[1]);
		arr.append(quad[2]);
		arr.append(quad[3]);
	}

	sf::Vertex quad[4];
	//quad = &arr[25 * 4];
	sf::Vector2f pos = sf::Vector2f(32, 32);
	quad[0].position = pos;
	quad[1].position = pos + sf::Vector2f(16, 0);
	quad[2].position = pos + sf::Vector2f(16, 16);
	quad[3].position = pos + sf::Vector2f(0, 16);
	sf::Vector2f txt = sf::Vector2f(9, 12) +  playerOffset;
	quad[0].texCoords = txt;
	quad[1].texCoords = txt + sf::Vector2f(16, 0);
	quad[2].texCoords = txt + sf::Vector2f(16, 16);
	quad[3].texCoords = txt + sf::Vector2f(0, 16);

	objects.append(quad[0]);
	objects.append(quad[1]);
	objects.append(quad[2]);
	objects.append(quad[3]);

	sf::RenderStates states;
	states.texture = &finalTxt;

	sf::RenderWindow window(sf::VideoMode(16 * 12 * 5, 16 * 9 * 5), "Game Project");
	
	sf::Transform playerTransform;
	sf::Transform everythingTransform;
	everythingTransform.scale(sf::Vector2f(4, 4));
	playerTransform.translate(sf::Vector2f(16, 16));

	states.transform *= everythingTransform;


	while (window.isOpen())	{
		// Handle window events.
		windowProcess(&window, in);

		if (in->isKeyDown(sf::Keyboard::D)) {
			for (size_t i = 0; i < 1 * 4; i++) {
				objects[i].position += sf::Vector2f(0.1, 0);
			}
		}

		arr.resize(25*4);
		for (size_t i = 0; i < 1 * 4; i++) {
			arr.append(objects[i]);
		}

		std::sort(arr.begin(), arr.end(), gameobjectsorting());

		window.clear();
		window.draw(arr, states);
		window.display();
	}
}
*/

float min(float a, float b) { return (a < b ? a : b); }
float max(float a, float b) { return (a > b ? a : b); }
bool pressed = false;

void windowProcess(sf::RenderWindow* window, InputHandler *in)
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

	//Create the window

	std::vector<Room*> rooms;
	rooms.push_back(new Forest_room(&window, in, "Levels/Level4/", "Level4.json"));
	//rooms.push_back(new Circle_room(&window, in, "Levels/Level1/", "Level1.json"));

	int current_room = 0;

	//rooms[0]->load();
	//rooms[1]->load();

	sf::Clock deltaClock;
	// Game Loop
	while (window.isOpen()) 	{
		// Handle window events.
		windowProcess(&window, in);

		float dt = deltaClock.restart().asSeconds();
		rooms[current_room]->handleInput(dt);
		rooms[current_room]->update(dt);
		rooms[current_room]->draw();
		//deltaClock.restart().asSeconds();
	}
}

