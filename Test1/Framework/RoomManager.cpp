#include "RoomManager.h"

RoomManager::RoomManager() : loadingThread(&RoomManager::loadMaps, this) {}

void RoomManager::setData(sf::RenderWindow* win, InputHandler* inp, sf::Clock* dt) {
	w = win; 
	in = inp; 
	deltaclock = dt;

	p = Player(in, this, w);

	enemydata.readJSON("GameObjects/enemy_data.json");

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
	rooms[map.data[map.currentRoom]]->setPlayer(&p);

	loadEnemies();
	loadMaps();

	p.setPosition(rooms[map.data[map.currentRoom]]->getOffset() + sf::Vector2f(16, 16));

	loadTextures();
}

RoomManager::~RoomManager() {
	delete in;
	in = nullptr;
	deltaclock = nullptr;
}

void RoomManager::loadEnemies() {
	if (!textures["enemy"].loadFromFile(enemydata.doc["spritesheet"].str)) {
		std::cout << "Couldn't load enemy spritesheet\n";
	}
	else {
		std::cout << "Loaded enemy spritesheet\n";
	}

	for (size_t i = 0; i < enemydata.doc["enemies"].arr.size(); i++) {
		Enemy e;
		e.setTexture(&textures["enemy"]);
		e.setInput(in);
		e.setWindow(w);
		e.setRoomManager(this);
		std::string name = enemydata.doc["enemies"].arr[i].obj["name"].str;
		int x = enemydata.doc["enemies"].arr[i].obj["collision"].obj["x"].i;
		int y = enemydata.doc["enemies"].arr[i].obj["collision"].obj["y"].i;
		int wi = enemydata.doc["enemies"].arr[i].obj["collision"].obj["width"].i;
		int h = enemydata.doc["enemies"].arr[i].obj["collision"].obj["height"].i;
		e.collisionlayer = Collision::LAYER::ENEMY;
		e.collider = Collision(x, y, wi, h, e.collisionlayer);
		
		int rows = enemydata.doc["rows"].i;
		int columns = enemydata.doc["columns"].i;
		AnimatedSprite animatedsprite;
		animatedsprite.setSpriteSheet(&textures["enemy"], columns, rows);
		for (size_t j = 0; j < enemydata.doc["enemies"].arr[i].obj["animations"].arr.size(); j++) {
			std::vector<int> animFrames;
			float speed = (float)enemydata.doc["enemies"].arr[i].obj["animations"].arr[j].obj["speed"].d;
			for (size_t k = 0; k < enemydata.doc["enemies"].arr[i].obj["animations"].arr[j].obj["array"].arr.size(); k++) {
				animFrames.push_back(enemydata.doc["enemies"].arr[i].obj["animations"].arr[j].obj["array"].arr[k].i);
			}
			animatedsprite.addAnimation(enemydata.doc["enemies"].arr[i].obj["animations"].arr[j].obj["name"].str, animFrames, speed);
		}
		animatedsprite.setCurrentAnimation(enemydata.doc["enemies"].arr[i].obj["animations"].arr[0].obj["name"].str);
		
		e.setAnimatedSprite(animatedsprite);
		e.setProjectile(enemydata.doc["enemies"].arr[i].obj["projectile"].b);
		e.setSpeed(enemydata.doc["enemies"].arr[i].obj["speed"].i);
		e.setLife(enemydata.doc["enemies"].arr[i].obj["life"].d);

		enemycopies[name] = e;
	}
}

void RoomManager::handleInput(float dt) {
	rooms[map.data[map.currentRoom]]->handleInput(dt);
}

void RoomManager::update(float dt) {
	if (!movingmap) rooms[map.data[map.currentRoom]]->update(dt);
	else animatetransition(dt);
}

void RoomManager::draw() {
	w->clear();
	if (movingmap) rooms[mapmovement.oldroom]->draw();
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
	if (map.currentRoom % map.width > 0 && map.data[(int)map.currentRoom - 1] != -1)
		toLoad.push_back({ map.currentRoom - 1, map.data[(int)map.currentRoom - 1] });
	// on the right
	if (map.currentRoom % map.width < map.width-1 && map.data[(int)map.currentRoom + 1] != -1)
		toLoad.push_back({ map.currentRoom + 1, map.data[(int)map.currentRoom + 1] });
	// on top
	if (map.currentRoom >= map.width && map.data[(int)map.currentRoom - map.width] != -1)
		toLoad.push_back({ map.currentRoom - map.width, map.data[(int)map.currentRoom - map.width] });
	// on bottom
	if (map.currentRoom / map.width < map.height - 1 && map.data[(int)map.currentRoom + map.width] != -1)
		toLoad.push_back({ map.currentRoom + map.width, map.data[(int)map.currentRoom + map.width] });
	
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

	int currentroom = map.data[map.currentRoom];

	rooms[oldcurrentroom]->removePlayer();
	rooms[currentroom]->setPlayer(&p);
	
	mapmovement.oldroom = oldcurrentroom;
	mapmovement.wasdebug = rooms[oldcurrentroom]->isdebug;
	rooms[oldcurrentroom]->isdebug = false;
	mapmovement.maincamera = maincamera;
	mapmovement.offsetdirection = offsetdirection;

	// LERPING
	// -- camera
	mapmovement.startCamera = maincamera.getCenter();
	mapmovement.targetCamera = mapmovement.startCamera + sf::Vector2f(maincamera.getSize().x * offsetdirection.x, maincamera.getSize().y * offsetdirection.y);
	mapmovement.cameratotaltime = 0.5f;
	mapmovement.cameratimeremaining = mapmovement.cameratotaltime;
	// -- player
	mapmovement.startPlayer = oldplayerpos - p.collider.collision_offset;
	mapmovement.targetPlayer = mapmovement.startPlayer + (17.f * offsetdirection);
	mapmovement.playertotaltime = 0.3f;
	mapmovement.playertimeremaining = mapmovement.playertotaltime;
	
	movingmap = true;
}

void RoomManager::animatetransition(float dt) {
	getCurrentRoom()->tilemap->animate(dt);
	rooms[mapmovement.oldroom]->tilemap->animate(dt);

	mapmovement.playertimeremaining -= dt;
	sf::Vector2f newplayerposition;
	float playerpercentagepassed = 1 - (mapmovement.playertimeremaining / mapmovement.playertotaltime);
	if (playerpercentagepassed < 1) {
		newplayerposition.x = UsefulFunc::lerp(mapmovement.startPlayer.x, mapmovement.targetPlayer.x, playerpercentagepassed);
		newplayerposition.y = UsefulFunc::lerp(mapmovement.startPlayer.y, mapmovement.targetPlayer.y, playerpercentagepassed);
		p.setPosition(newplayerposition);
		std::cout << newplayerposition.x << " " << newplayerposition.y << "\n";
	}

	mapmovement.cameratimeremaining -= dt;
	sf::Vector2f newcameraposition;
	float camerapercentagepassed = 1 - (mapmovement.cameratimeremaining / mapmovement.cameratotaltime);
	newcameraposition.x = UsefulFunc::lerp(mapmovement.startCamera.x, mapmovement.targetCamera.x, camerapercentagepassed);
	newcameraposition.y = UsefulFunc::lerp(mapmovement.startCamera.y, mapmovement.targetCamera.y, camerapercentagepassed);
	mapmovement.maincamera.setCenter(newcameraposition);
	w->setView(mapmovement.maincamera);
	if (camerapercentagepassed >= 1) {
		getCurrentRoom()->isdebug = mapmovement.wasdebug;
		getCurrentRoom()->setMainCamera(mapmovement.maincamera);
		movingmap = false;
	}
}