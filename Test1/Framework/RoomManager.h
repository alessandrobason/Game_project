#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <unordered_map>
#include "InputHandler.h"
#include "Tilemap.h"
#include "Tweening.h"
#include "../JSONparser/JSONparser.h"
#include "RoomManager.fwd.h"
#include "../Map_room.h"
#include "MainScreen.h"
#include "SplashScreens.h"

class RoomManager {
public:
	enum RoomSides {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};
	enum class STATES {
		MENU,
		MAP,
		MAPTRANSITION,
		MENUTRANSITION
	};

private:
	struct Map {
		int width = 0;
		int height = 0;
		int currentRoom = -1;
		std::vector<int> data;
		std::vector<std::string> files;
	} map;

	struct MapMovement {
		int oldroom;
		sf::View maincamera;
		sf::Vector2f offsetdirection;
		Tweening<sf::Vector2f> playertween;
		Tweening<sf::Vector2f> cameratween;
		bool wasdebug;
	} mapmovement;

	struct MenuTransition {
		Tweening<sf::Vector2f> uppersquaretween;
		Tweening<sf::Vector2f> bottomsquaretween;
		sf::RectangleShape uppersquare;
		sf::RectangleShape bottomsquare;
		std::string newmenu;
		bool actually_finished = false;
	} menutransitiondata;

	STATES currentstate;

	std::map<std::string, Enemy> enemycopies;

	Player p;
	std::vector<Map_room*> maprooms;
	std::string currentroom = "";
	InputHandler* in = nullptr;
	sf::RenderWindow* w = nullptr;
	
	std::unordered_map<std::string, Room*> menuscreens;
	std::string currentmenu = "";
	//Room* mainscreen = nullptr;
	//Room* splashscreen = nullptr;

	JSONparser enemydata;

	//bool movingmap = false;

	void loadMaps();
	void loadTextures();

	sf::Thread loadingThread;
public:

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
	void moveMenu(std::string newmenu);

	void animatetransition(float dt);
	void menuTransition(float dt);
	void drawMenuTransition();

	void loadEnemies();

	void setCurrentState(STATES s) { currentstate = s; }
	STATES getCurrentState() { return currentstate; }
	void setCurrentMenu(std::string m) { currentmenu = m; }
	std::string getCurrentMenu() { return currentmenu; }

	// dictionary with all the textures of the game
	std::map<std::string, sf::Texture> textures;
	std::vector<Image> images;
	// dictionary with all the fonts of the game
	std::map<std::string, sf::Font> fonts;
	// dictionary with all the maps currently loaded
	std::map<std::string, MapData> tilemapData;

	int MAPSIZE = 16 * 40;
};
