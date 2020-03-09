#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "InputHandler.h"
#include "Tilemap.h"
#include "Tweening.h"
#include "../JSONparser/JSONparser.h"
#include "RoomManager.fwd.h"
#include "../Map_room.h"
#include "MainScreen.h"

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
		Tweening<sf::Vector2f> playertween;
		Tweening<sf::Vector2f> cameratween;
		bool wasdebug;
	};

	enum class STATES {
		MAINSCREEN,
		MAP,
		MAPTRANSITION
	};

	STATES currentstate;

	std::map<std::string, Enemy> enemycopies;

	MapMovement mapmovement;

	Player p;
	Map map;
	std::vector<Map_room*> maprooms;
	MainScreen* mainscreen = nullptr;
	//std::map<std::string, Room*> rooms;
	std::string currentroom = "";
	InputHandler* in = nullptr;
	sf::RenderWindow* w = nullptr;
	//sf::Clock* deltaclock = nullptr;

	JSONparser enemydata;

	//bool movingmap = false;

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

	void setData(sf::RenderWindow* win, InputHandler* i);

	void handleInput(float dt);
	void update(float dt);
	void draw();

	Map_room* getCurrentRoom() { return maprooms[map.data[map.currentRoom]]; }
	JSONparser* getEnemyData() { return &enemydata; }
	Enemy& getEnemyCopy(std::string enemy) { return enemycopies[enemy]; }

	void moveRoom(int side);

	void animatetransition(float dt);

	void loadEnemies();

	// dictionary with all the textures of the game
	std::map<std::string, sf::Texture> textures;
	std::vector<Image> images;
	// dictionary with all the maps currently loaded
	std::map<std::string, MapData> tilemapData;

	int MAPSIZE = 16 * 40;
};
