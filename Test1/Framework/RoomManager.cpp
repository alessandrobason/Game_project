#include "RoomManager.h"

RoomManager::RoomManager(sf::RenderWindow* win, InputHandler* i) {
	w = win;
	in = i;
	JSONparser worldmap = JSONparser("Levels/worldmap.json");
	map.width = worldmap.doc["width"].i;
	map.height = worldmap.doc["height"].i;

	for (size_t i = 0; i < worldmap.doc["Levels"].arr.size(); i++) {
		map.files.push_back(worldmap.doc["Levels"].arr[i].str);
		rooms.push_back(new Forest_room(this, w, in, map.files[i]));
	}
	for (size_t i = 0; i < worldmap.doc["map"].arr.size(); i++) {
		map.data.push_back(worldmap.doc["map"].arr[i].i);
	}
	map.currentRoom = map.data[worldmap.doc["spawn"].i];

	if (!textures["player"].loadFromFile("Assets/link.png")) {

	}
}

RoomManager::~RoomManager() {
	delete in;
	in = nullptr;
	delete deltaclock;
	deltaclock = nullptr;
}

void RoomManager::handleInput(float dt) {
	rooms[map.currentRoom]->handleInput(dt);
}

void RoomManager::update(float dt) {
	rooms[map.currentRoom]->update(dt);
}

void RoomManager::draw() {
	w->clear();
	rooms[map.currentRoom]->draw();
	w->display();
}

void RoomManager::loadMaps() {
	std::vector<int> toLoad;
	toLoad.push_back(map.currentRoom);
	// on the left
	if (map.currentRoom > 0) 
		toLoad.push_back(map.data[map.currentRoom - 1]);
	// on the right
	if (map.currentRoom % map.width < map.width-1)
		toLoad.push_back(map.data[map.currentRoom + 1]);
	// on top
	if (map.currentRoom >= map.width)
		toLoad.push_back(map.data[map.currentRoom - map.width]);
	// on bottom
	if (map.currentRoom / map.width < map.height - 1)
		toLoad.push_back(map.data[map.currentRoom + map.width]);

	std::cout << "rrr: " << rooms.size() << "\n";

	//p = Player(in, rooms[map.currentRoom], sf::Vector2f(0, 0));
	//Player* pl = new Player(in, rooms[map.currentRoom], sf::Vector2f(64, 64));
	//pl->setSpeed(100);
	p = Player(in, rooms[map.currentRoom], sf::Vector2f(64, 64));
	p.setSpeed(100);
	rooms[map.currentRoom]->setPlayer(&p);
	for (size_t i = 0; i < toLoad.size(); i++) {
		/*
		##############################
		##############################

		DA FARE:
		- FARE UNA MAPPA DELLE ROOMS IN MODO CHE SI POSSA
		  CHIAMARE rooms[map.files[toLoad[i]]]

		##############################
		##############################
		*/
		std::cout << "load: " << toLoad[i] << "\n";
		sf::Vector2f offset;
		if (i > 0) {
			offset = rooms[toLoad[i-1]]->getOffset() + sf::Vector2f(MAPSIZE, 0);
		}
		//offset = sf::Vector2f(0, 0);
		rooms[toLoad[i]]->load(-offset);
		
		if (toLoad[i] != map.currentRoom) {

		}
			//rooms[toLoad[i]]->removePlayer();
	}
	//rooms[map.currentRoom]->load();
	//rooms[map.currentRoom]->setPlayer(&p);
	std::cout << "loaded?";
}

void RoomManager::moveRoom(int side) {
	int oldcurrentroom = map.currentRoom;
	sf::View maincamera = rooms[oldcurrentroom]->getMainCamera();
	sf::Vector2f oldplayerpos = rooms[oldcurrentroom]->getPlayerPosition();
	Player::DIRECTIONS lastplayerdirection = p.getDirection();
	sf::Vector2f offsetdirection;
	sf::FloatRect bound;
	switch (side) {
	case TOP:
		break;
	case BOTTOM:
		break;
	case LEFT:
		bound = rooms[oldcurrentroom]->getBound(side);
		if (map.currentRoom == 0) return;
		map.currentRoom--;
		offsetdirection = sf::Vector2f(-1, 0);
		break;
	case RIGHT:
		if (map.currentRoom == rooms.size()-1) return;
		//if (map.currentRoom == 0) return;
		map.currentRoom++;
		offsetdirection = sf::Vector2f(1, 0);
		break;
	}
	

	p = Player(in, rooms[map.currentRoom], sf::Vector2f(64, 64));
	p.setSpeed(100);

	rooms[oldcurrentroom]->removePlayer();
	rooms[map.currentRoom]->setPlayer(&p);
	rooms[map.currentRoom]->setPlayerPosition(oldplayerpos);
	tilemapData[rooms[oldcurrentroom]->getFolder()].tilemap.resetAnimation();
	//sf::Vector2f middleplayerpos = rooms[map.currentRoom]->getPlayerPosition();
	//std::cout << "middle: " << middleplayerpos.x << " " << middleplayerpos.y << "\n";

	sf::Transform t;
	t.translate(offsetdirection * 16.f * 40.f);
	//rooms[map.currentRoom]->moveRoom(t);
	//sf::Vector2f targetPosition = tilemapData[rooms[oldcurrentroom]->getFolder()].tilemap.getPosition();

	//std::cout << targetPosition.x << " --- " << targetPosition.y << "\n";

	bool transitioning = true;
	bool movingplayer = true;
	float speed = 200;

	//sf::View help = maincamera;

	sf::Vector2f playeroldposition = p.getSprite()->getPosition();
	p.setDirection(lastplayerdirection);
	p.vel = sf::Vector2f(0, 0);

	sf::Clock deltaClock;
	while (transitioning) {
		//std::cout << "fuck\n";
		w->clear();
		float dt = deltaClock.restart().asSeconds();
		tilemapData[rooms[oldcurrentroom]->getFolder()].tilemap.animate(dt);
		tilemapData[rooms[map.currentRoom]->getFolder()].tilemap.animate(dt);
		//p.update(dt);
		
		rooms[oldcurrentroom]->draw();
		rooms[map.currentRoom]->draw();
		w->display();

		maincamera.move(offsetdirection*dt*speed);
		if (movingplayer) {
			p.move(offsetdirection * dt * speed);
		}
		w->setView(maincamera);
		switch (side) {
		case TOP:
			return;
			break;
		case BOTTOM:
			return;
			break;
		case LEFT:
			if (maincamera.getCenter().x + (maincamera.getSize().x / 2) <=  bound.left) {
				transitioning = false;
			}
			if (p.collider.rect.left <= oldplayerpos.x - 17) {
				movingplayer = false;
			}
			break;
		case RIGHT:
			if (maincamera.getCenter().x - (maincamera.getSize().x / 2) >= bound.left + bound.width) {
				transitioning = false;
			}
			if (p.collider.rect.left >= oldplayerpos.x + 17) {
				movingplayer = false;
			}
			break;
		}		
	}
	std::cout << maincamera.getCenter().x << " " << maincamera.getCenter().y << "\n";
	deltaclock->restart();
}