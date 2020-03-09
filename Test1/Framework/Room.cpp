#include "Room.h"
#include "RoomManager.h"

Room::Room(std::string fold, sf::RenderWindow* window, InputHandler* input, RoomManager* rm){
	FOLDER = fold;
	std::cout << FOLDER + "/" + FOLDER + ".json" << "\n";
	config = new JSONparser("Rooms/" + FOLDER + "/" + FOLDER + ".json");

	in = input;
	w = window;
	roomManager = rm;

	main_camera = in->getView();
	w->setView(main_camera);

	images = &roomManager->images;
	isloaded = &roomManager->tilemapData[fold].loaded;
}

Room::~Room() {
	delete config;
	config = nullptr;
}

void Room::handleInput(float dt) {}
void Room::update(float dt) {}
void Room::draw() {}

void Room::load(sf::Vector2f offs) {}