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
	Room() {}
	Room(std::string fold, sf::RenderWindow* window, InputHandler* input, RoomManager* rm);
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
	JSONparser* config = nullptr;

	struct gameobjectsorting {
		inline bool operator() (GameObject* g1, GameObject* g2) {
			return g1->getY() < g2->getY();
		}
	};

protected:
	// input handler
	InputHandler* in = nullptr;
	// render window
	sf::RenderWindow* w = nullptr;

	// room manager
	RoomManager* roomManager = nullptr;

	// useful information
	std::string FOLDER;
	const std::string ASSETS = "Assets/";


	// main camera
	sf::View main_camera;


	// vectors with scene objects/colliders
	std::vector<GameObject*> sceneObjects;
	std::vector<GameObject*> destroyedSceneObjects;
	std::vector<GameObject*> invisibleSceneObjects;
	std::vector<Collision*> sceneColliders;
	std::vector<Collision*> invisibleSceneColliders;

};
