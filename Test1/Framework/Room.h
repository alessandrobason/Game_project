#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "../JSONparser/JSONparser.h"
#include "Tilemap.h"
#include "InputHandler.h"

class Room {
public:
	Room(std::string fold, std::string config_path, InputHandler* input);
	~Room();

	virtual void handleInput(float dt);
	virtual void update(float dt);
	virtual void draw();
	virtual void load();

	JSONparser* config;

protected:
	// render window
	sf::RenderWindow* w;

	// useful information
	std::string FOLDER;
	const std::string ASSETS = "Assets/";

	// tilemap data
	std::vector<int> layers;
	Tilemap::tilemap_data tilemap_data;
	Tilemap tilemap;
	sf::Shader shader;

	// main camera
	sf::View main_camera;

	// input handler
	InputHandler* in;
};
