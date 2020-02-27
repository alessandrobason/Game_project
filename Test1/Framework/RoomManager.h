#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
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

	Player p;
	Map map;
	std::vector<Forest_room*> rooms;
	InputHandler* in;
	sf::RenderWindow* w;
	sf::Clock* deltaclock = 0;

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

	void moveRoom(int side);

	// dictionary with all the textures of the game
	std::map<std::string, sf::Texture> textures;
	std::vector<Image> images;
	// dictionary with all the maps currently loaded
	std::map<std::string, MapData> tilemapData;

	int MAPSIZE = 16 * 40;
};
