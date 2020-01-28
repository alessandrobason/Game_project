#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "JSONparser/JSONparser.h"
#include "Tilemap.h"

class Room {
public:
	Room(std::string fold, std::string config_path);
	~Room();

	virtual void handle_input();
	virtual void update();
	virtual void draw();
	virtual void load();

	virtual void ciao() {
		std::cout << "ciao\n";
	}

	JSONparser* config;

protected:
	sf::RenderWindow* w;

	std::vector<int> layers;
	Tilemap::tilemap_data tilemap_data;
	Tilemap tilemap;
	sf::Shader shader;

	sf::View *main_camera;

	std::string FOLDER;
	const std::string ASSETS = "Assets/";
};
