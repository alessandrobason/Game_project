#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "../JSONparser/JSONparser.h"
#include "InputHandler.h"
#include "GameObject.h"
#include "Tilemap.h"
#include "Room.fwd.h"
#include "RoomManager.fwd.h"

class Room {
public:
	Room(std::string fold, InputHandler* input, RoomManager* rm);
	~Room();

	virtual void handleInput(float dt);
	virtual void update(float dt);
	virtual void draw();
	virtual void load(sf::Vector2f offset);

	sf::View getMainCamera() { return main_camera; }
	std::string getFolder() { return FOLDER; }

	void setMainCamera(sf::View c) { main_camera = c; }

	// dictionary with all the textures of the scene
	//std::map<std::string, sf::Texture>* textures = nullptr;
	std::vector<Image>* images = nullptr;

	// if true: draw collisionboxes ecc
	bool isdebug = false;

	bool* isloaded = nullptr;
	Tilemap* tilemap = nullptr;

	JSONparser* config;

	struct gameobjectsorting {
		inline bool operator() (GameObject* g1, GameObject* g2) {
			return g1->getY() < g2->getY();
		}
	};

protected:
	// render window
	sf::RenderWindow* w;

	// room manager
	RoomManager* roomManager = nullptr;

	// useful information
	std::string FOLDER;
	const std::string ASSETS = "Assets/";

	// tilemap data
	JSONparser* tilemap_json;
	sf::Vector2f offset;
	std::vector<int>* layers = nullptr;
	Tilemap::tilemap_data* tilemap_data = nullptr;
	sf::Shader* shader = nullptr;

	// main camera
	sf::View main_camera;

	// input handler
	InputHandler* in;

	// vectors with scene objects/colliders
	std::vector<GameObject*> sceneObjects;
	std::vector<GameObject*> invisibleSceneObjects;
	std::vector<Collision*> sceneColliders;
	std::vector<Collision*> invisibleSceneColliders;

};
