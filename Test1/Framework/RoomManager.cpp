#include "RoomManager.h"

RoomManager::RoomManager() : loadingThread(&RoomManager::loadMaps, this) {
	//sf::Thread loadThread(&RoomManager::loadMaps, this);
}

void RoomManager::setData(sf::RenderWindow* win, InputHandler* inp, sf::Clock* dt) {
	w = win; 
	in = inp; 
	deltaclock = dt;

	p = Player(in, this, w);
	//p.setPosition(sf::Vector2f(16, 16));
	p.setSpeed(100);

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
	map.currentRoom = worldmap.doc["spawn"].i;

	if (!textures["player"].loadFromFile("Assets/link.png")) {
		std::cout << "Couldn't load player texture";
	}

	rooms[map.data[map.currentRoom]]->setPlayer(&p);
	loadingThread.launch();
	loadingThread.wait();
	p.setPosition(rooms[map.data[map.currentRoom]]->getOffset() + sf::Vector2f(16, 16));
	loadTextures();
}

RoomManager::~RoomManager() {
	delete in;
	in = nullptr;
	deltaclock = nullptr;
}

void RoomManager::handleInput(float dt) {
	rooms[map.data[map.currentRoom]]->handleInput(dt);
}

void RoomManager::update(float dt) {
	rooms[map.data[map.currentRoom]]->update(dt);
}

void RoomManager::draw() {
	w->clear();
	rooms[map.data[map.currentRoom]]->draw();
	w->display();
}

void RoomManager::loadMaps() {
	struct load {
		int position;
		int map;
	};
	std::vector<load> toLoad;
	toLoad.push_back({ map.currentRoom, map.data[map.currentRoom] });
	// on the left
	if (map.currentRoom % map.width > 0 && map.data[map.currentRoom - 1] != -1)
		toLoad.push_back({ map.currentRoom - 1, map.data[map.currentRoom - 1] });
	// on the right
	if (map.currentRoom % map.width < map.width-1 && map.data[map.currentRoom + 1] != -1)
		toLoad.push_back({ map.currentRoom + 1, map.data[map.currentRoom + 1] });
	// on top
	if (map.currentRoom >= map.width && map.data[map.currentRoom - map.width] != -1)
		toLoad.push_back({ map.currentRoom - map.width, map.data[map.currentRoom - map.width] });
	// on bottom
	if (map.currentRoom / map.width < map.height - 1 && map.data[map.currentRoom + map.width] != -1)
		toLoad.push_back({ map.currentRoom + map.width, map.data[map.currentRoom + map.width] });
	
	for (size_t i = 0; i < toLoad.size(); i++) {
		if (*rooms[toLoad[i].map]->isloaded == true) continue;
		int mapx = toLoad[i].position % map.width;
		int mapy = toLoad[i].position / map.width;
		std::cout << "load: " << toLoad[i].map << "\n";
		sf::Vector2f offset;
		offset = sf::Vector2f(mapx * MAPSIZE, mapy * MAPSIZE);
		
		rooms[toLoad[i].map]->load(offset);
	}
}

void RoomManager::loadTextures() {
	for (size_t i = 0; i < images.size(); i++) {
		if (!textures[images[i].key].loadFromImage(images[i].img)) {
			std::cout << "couldn't load " << images[i].key << "\n";
		}
	}
	for (size_t i = 0; i < rooms.size(); i++) {
		if (!rooms[i]->isloaded) continue;
		rooms[i]->tilemap->setTexture(&textures["tiles"]);
	}
}

void RoomManager::moveRoom(int side) {
	int oldcurrentroom = map.data[map.currentRoom];
	sf::View maincamera = rooms[oldcurrentroom]->getMainCamera();
	sf::Vector2f oldplayerpos = rooms[oldcurrentroom]->getPlayerPosition();
	Player::DIRECTIONS lastplayerdirection = p.getDirection();
	sf::Vector2f offsetdirection;
	sf::FloatRect bound;
	switch (side) {
	case TOP:
		if (map.currentRoom - map.width  < 0) return;
		map.currentRoom -= map.width;
		offsetdirection = sf::Vector2f(0, -1);
		break;
	case BOTTOM:
		if (map.currentRoom + map.width >= map.data.size()) return;
		map.currentRoom += map.width;
		offsetdirection = sf::Vector2f(0, +1);
		break;
	case LEFT:
		if (map.currentRoom%map.width == 0) return;
		map.currentRoom--;
		offsetdirection = sf::Vector2f(-1, 0);
		break;
	case RIGHT:
		if ((map.currentRoom%map.width)+1 >= map.width) return;
		map.currentRoom++;
		offsetdirection = sf::Vector2f(1, 0);
		break;
	}
	loadingThread.launch();
	bound = rooms[oldcurrentroom]->getBound(side);
	int currentroom = map.data[map.currentRoom];
	//p = Player(in, rooms[currentroom], oldplayerpos);
	//p.setSpeed(100);

	rooms[oldcurrentroom]->removePlayer();
	rooms[currentroom]->setPlayer(&p);
	tilemapData[rooms[oldcurrentroom]->getFolder()].tilemap.resetAnimation();

	/*
	sf::Transform t;
	sf::Vector2f offset = offsetdirection * (float)MAPSIZE;
	offset += rooms[oldcurrentroom]->getOffset();
	t.translate(offset);
	rooms[currentroom]->moveRoom(t);
	rooms[currentroom]->setBounds(offset);
	*/

	bool transitioning = true;
	bool movingplayer = true;
	float speed = 200;

	sf::Vector2f playeroldposition = p.getSprite()->getPosition();
	p.setDirection(lastplayerdirection);
	p.vel = sf::Vector2f(0, 0);

	bool wasdebug = rooms[oldcurrentroom]->isdebug;
	rooms[oldcurrentroom]->isdebug = false;

	sf::Clock deltaClock;
	while (transitioning) {
		//std::cout << "fuck\n";
		w->clear();
		float dt = deltaClock.restart().asSeconds();
		tilemapData[rooms[oldcurrentroom]->getFolder()].tilemap.animate(dt);
		tilemapData[rooms[currentroom]->getFolder()].tilemap.animate(dt);
		//p.update(dt);
		
		rooms[oldcurrentroom]->draw();
		rooms[currentroom]->draw();

		maincamera.move(offsetdirection*dt*speed);
		if (movingplayer) {
			p.move(offsetdirection * dt * speed);
		}
		w->setView(maincamera);
		std::cout << maincamera.getCenter().x << " " << maincamera.getCenter().y << "\n";
		w->display();

		switch (side) {
		case TOP:
			if (maincamera.getCenter().y + (maincamera.getSize().y / 2) <= bound.top) {
				transitioning = false;
			}
			if (p.collider.rect.top <= oldplayerpos.y - 17) {
				movingplayer = false;
			}
			break;
		case BOTTOM:
			if (maincamera.getCenter().y - (maincamera.getSize().y / 2) >= bound.top + bound.height) {
				transitioning = false;
			}
			if (p.collider.rect.top >= oldplayerpos.y + 17) {
				movingplayer = false;
			}
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
	std::cout << "\n-->\n";
	std::cout << maincamera.getCenter().x << " " << maincamera.getCenter().y << "\n";
	rooms[currentroom]->isdebug = wasdebug;
	rooms[currentroom]->setMainCamera(maincamera);
	deltaclock->restart();
}