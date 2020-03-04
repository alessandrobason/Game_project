#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Tween.h"
#include "InputHandler.h"
#include "Tilemap.h"
#include "../JSONparser/JSONparser.h"
#include "RoomManager.fwd.h"
#include "Room.fwd.h"
#include "../Forest_room.h"

class RoomManager {
	struct Map {
		int width = 0;
		int height = 0;
		int currentRoom = -1;
		std::vector<int> data;
		std::vector<std::string> files;
	};

	struct MapMovement {
		int oldroom;
		sf::View maincamera;
		sf::Vector2f offsetdirection;
		sf::Vector2f startCamera;
		sf::Vector2f targetCamera;
		sf::Vector2f startPlayer;
		sf::Vector2f targetPlayer;
		float cameratotaltime;
		float cameratimeremaining;
		float playertotaltime;
		float playertimeremaining;
		bool wasdebug;
	};

	MapMovement mapmovement;

	Player p;
	Map map;
	std::vector<Forest_room*> rooms;
	InputHandler* in = nullptr;
	sf::RenderWindow* w = nullptr;
	sf::Clock* deltaclock = nullptr;

	bool movingmap = false;

	void loadMaps();
	void loadTextures();

	sf::Thread loadingThread;
public:
	enum RoomSides {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};

	RoomManager();
	~RoomManager();

	void setData(sf::RenderWindow* win, InputHandler* i, sf::Clock* dt);

	void handleInput(float dt);
	void update(float dt);
	void draw();

	Forest_room* getCurrentRoom() { return rooms[map.data[map.currentRoom]]; }

	void moveRoom(int side);

	void animatetransition(float dt);

	// dictionary with all the textures of the game
	std::map<std::string, sf::Texture> textures;
	std::vector<Image> images;
	// dictionary with all the maps currently loaded
	std::map<std::string, MapData> tilemapData;

	int MAPSIZE = 16 * 40;
};
