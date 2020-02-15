#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "../JSONparser/JSONparser.h"
#include "InputHandler.h"
#include "GameObject.h"
#include "Tilemap.h"

class Room {
public:
	Room(std::string fold, std::string config_path, InputHandler* input);
	~Room();

	virtual void handleInput(float dt);
	virtual void update(float dt);
	virtual void draw();
	virtual void load();

	void sortGameObjects();

	// dictionary with all the textures of the scene
	std::map<std::string, sf::Texture> textures;

	// if true: draw collisionboxes ecc
	bool isdebug = false;

	JSONparser* config;

	struct gameobjectsorting {
		inline bool operator() (GameObject* g1, GameObject* g2) {
			return g1->getY() < g2->getY();
		}
	};

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

	// vector with all the gameobjects of the scene
	std::vector<GameObject*> sceneObjects;


};
