#include "Room.h"
#include "RoomManager.h"

Room::Room(std::string fold, sf::RenderWindow* window, InputHandler* input, RoomManager* rm){
	FOLDER = fold;
	std::cout << FOLDER + "/" + FOLDER + ".json" << "\n";
	config = new JSONparser("Rooms/" + FOLDER + "/" + FOLDER + ".json");

	in = input;
	w = window;
	roommanager = rm;

	main_camera = in->getView();
	w->setView(main_camera);

	images = &roommanager->images;
	isloaded = &roommanager->tilemapData[fold].loaded;
}

Room::~Room() {
	delete config;
	config = nullptr;
}

void Room::handleInput(float dt) {}
void Room::update(float dt) {}
void Room::draw() {}

void Room::load(sf::Vector2f offs) {}