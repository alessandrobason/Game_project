#include "RoomManager.h"

RoomManager::RoomManager() : loadingThread(&RoomManager::loadMaps, this) {
	currentstate = STATES::MENU;
	currentmenu = "splashscreen";
	if (!musics["menu"].openFromFile("Assets/music/Bill Evans - Someday My Prince Will Come.ogg")) {
		std::cout << "couldn't open music file for menu\n";
		abort();
	}
	musics["menu"].setLoop(true);
}

void RoomManager::setData(sf::RenderWindow* win, InputHandler* inp) {
	w = win; 
	in = inp; 
	
	menutransitiondata.uppersquaretween = Tweening<sf::Vector2f>(sf::Vector2f(in->getScreenSize().x, 0), sf::Vector2f(in->getScreenSize().x, in->getScreenSize().y / 2), 0.5f);
	menutransitiondata.bottomsquaretween = Tweening<sf::Vector2f>(sf::Vector2f(in->getScreenSize().x, 0), sf::Vector2f(in->getScreenSize().x, -in->getScreenSize().y / 2), 0.5f);

	menutransitiondata.uppersquare = sf::RectangleShape(sf::Vector2f(in->getScreenSize().x, 0));
	menutransitiondata.uppersquare.setPosition(sf::Vector2f());
	menutransitiondata.uppersquare.setFillColor(sf::Color::Black);
	menutransitiondata.bottomsquare = sf::RectangleShape(sf::Vector2f(in->getScreenSize().x, 0));
	menutransitiondata.bottomsquare.setPosition(sf::Vector2f(0, in->getScreenSize().y));
	menutransitiondata.bottomsquare.setFillColor(sf::Color::Black);

	menuscreens["splashscreen"] = new SplashScreens(w, in, this);
	menuscreens["startscreen"] = new MainScreen(w, in, this);
	menuscreens["mainmenu"] = new MainMenu(w, in, this);
	menuscreens["options"] = new OptionMenu(w, in, this);
	
	p = Player(in, this, w);
	//p.setCanMove(false);

	enemydata.readJSON("GameObjects/enemy_data.json");

	JSONparser worldmap = JSONparser("Rooms/worldmap.json");
	map.width = worldmap.doc["width"].i;
	map.height = worldmap.doc["height"].i;

	for (size_t i = 0; i < worldmap.doc["Levels"].arr.size(); i++) {
		map.files.push_back(worldmap.doc["Levels"].arr[i].str);
		maprooms.push_back(new Map_room(this, w, in, map.files[i]));
	}
	for (size_t i = 0; i < worldmap.doc["map"].arr.size(); i++) {
		map.data.push_back(worldmap.doc["map"].arr[i].i);
	}

	map.currentRoom = worldmap.doc["spawn"].i;
	maprooms[map.data[map.currentRoom]]->setPlayer(&p);

	loadEnemies();
	loadMaps();

	p.setPosition(maprooms[map.data[map.currentRoom]]->getOffset() + sf::Vector2f(16, 16));

	loadTextures();
}

RoomManager::~RoomManager() {
	delete in;
	in = nullptr;
}

void RoomManager::loadEnemies() {
	if (!textures["enemy"].loadFromFile(enemydata.doc["spritesheet"].str)) {
		std::cout << "Couldn't load enemy spritesheet\n";
	}
	else {
		std::cout << "Loaded enemy spritesheet\n";
	}

	if (!textures["enemyproj"].loadFromFile("Assets/enemyprojectiles.png")) {
		std::cout << "couldn't load enemies projectiles\n";
	}
	else {
		std::cout << "Loaded enemies projectiles texture\n";
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
	switch (currentstate) {
	case RoomManager::STATES::MENU:
		menuscreens[currentmenu]->handleInput(dt);
		break;
	case RoomManager::STATES::MAP:
		maprooms[map.data[map.currentRoom]]->handleInput(dt);
		break;
	}
}

void RoomManager::update(float dt) {
	switch (currentstate) {
	case RoomManager::STATES::MENU:
		menuscreens[currentmenu]->update(dt);
		break;
	case RoomManager::STATES::MAP:
		maprooms[map.data[map.currentRoom]]->update(dt);
		break;
	case RoomManager::STATES::MAPTRANSITION:
		animatetransition(dt);
		break;
	case RoomManager::STATES::MENUTRANSITION:
		//if (currentmenu == "game") maprooms[map.data[map.currentRoom]]->update(dt);
		menuTransition(dt);
		break;
	}
}

void RoomManager::draw() {
	w->clear();
	switch (currentstate) {
	case RoomManager::STATES::MENU:
		menuscreens[currentmenu]->draw();
		break;
	case RoomManager::STATES::MAP:
		maprooms[map.data[map.currentRoom]]->draw();
		break;
	case RoomManager::STATES::MAPTRANSITION:
		maprooms[mapmovement.oldroom]->draw();
		maprooms[map.data[map.currentRoom]]->draw();
		break;
	case RoomManager::STATES::MENUTRANSITION:
		if(currentmenu == "game") maprooms[map.data[map.currentRoom]]->draw();
		else menuscreens[currentmenu]->draw();
		drawMenuTransition();
		break;
	}
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
		if (*maprooms[toLoad[i].map]->isloaded == true) continue;
		int mapx = toLoad[i].position % map.width;
		int mapy = toLoad[i].position / map.width;
		std::cout << "load: " << toLoad[i].map << "\n";
		sf::Vector2f offset;
		offset = sf::Vector2f(mapx * MAPSIZE, mapy * MAPSIZE);
		
		maprooms[toLoad[i].map]->load(offset);
	}
}

void RoomManager::loadTextures() {
	for (size_t i = 0; i < images.size(); i++) {
		if (!textures[images[i].key].loadFromImage(images[i].img)) {
			std::cout << "couldn't load " << images[i].key << "\n";
		}
	}
	for (size_t i = 0; i < maprooms.size(); i++) {
		if (!maprooms[i]->isloaded) continue;
		maprooms[i]->getTilemap()->setTexture(&textures["tiles"]);
	}
}

void RoomManager::moveRoom(int side) {
	int oldcurrentroom = map.data[map.currentRoom];
	sf::View maincamera = maprooms[oldcurrentroom]->getMainCamera();
	sf::Vector2f oldplayerpos = maprooms[oldcurrentroom]->getPlayerPosition();
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

	maprooms[oldcurrentroom]->removePlayer();
	maprooms[currentroom]->setPlayer(&p);
	
	mapmovement.oldroom = oldcurrentroom;
	mapmovement.wasdebug = maprooms[oldcurrentroom]->isdebug;
	maprooms[oldcurrentroom]->isdebug = false;
	mapmovement.maincamera = maincamera;
	mapmovement.offsetdirection = offsetdirection;

	// LERPING
	// -- camera
	sf::Vector2f finalcamera = maincamera.getCenter() + sf::Vector2f(maincamera.getSize().x * offsetdirection.x, maincamera.getSize().y * offsetdirection.y);
	mapmovement.cameratween = Tweening<sf::Vector2f>(maincamera.getCenter(), finalcamera, 0.5f);
	// -- player
	sf::Vector2f startPlayer = oldplayerpos - p.collider.collision_offset;
	sf::Vector2f finalplayer = startPlayer + (17.f * offsetdirection);
	mapmovement.playertween = Tweening<sf::Vector2f>(startPlayer, finalplayer, 0.3f);
	
	currentstate = STATES::MAPTRANSITION;
}

void RoomManager::animatetransition(float dt) {
	mapmovement.playertween.update(dt);
	mapmovement.cameratween.update(dt);
	
	sf::Vector2f newplayerposition;
	if (!mapmovement.playertween.isfinished()) {
		newplayerposition = mapmovement.playertween.getValue();
		p.setPosition(newplayerposition);
	}

	sf::Vector2f newcameraposition;
	newcameraposition = mapmovement.cameratween.getValue();
	mapmovement.maincamera.setCenter(newcameraposition);
	w->setView(mapmovement.maincamera);
	if (mapmovement.cameratween.isfinished()) {
		getCurrentRoom()->isdebug = mapmovement.wasdebug;
		getCurrentRoom()->setMainCamera(mapmovement.maincamera);
		currentstate = STATES::MAP;
	}
}

void RoomManager::moveMenu(std::string newmenu) {
	lastmenu = currentmenu;
	menutransitiondata.newmenu = newmenu;
	currentstate = STATES::MENUTRANSITION;
	setMenuTransitionOffset(currentmenu);
	/*
	sf::Vector2f offsetU = sf::Vector2f();
	sf::Vector2f offsetB = sf::Vector2f(0, in->getScreenSize().y);
	if (newmenu == "game") {
		offsetU = getCurrentRoom()->getCameraTopLeft();
		offsetB += offsetU;
	}
	menutransitiondata.uppersquare.setPosition(offsetU);
	menutransitiondata.bottomsquare.setPosition(offsetB);
	*/
}

void RoomManager::menuTransition(float dt) {
	menutransitiondata.uppersquaretween.update(dt);
	menutransitiondata.bottomsquaretween.update(dt);

	menutransitiondata.uppersquare.setSize(menutransitiondata.uppersquaretween.getValue());
	menutransitiondata.bottomsquare.setSize(menutransitiondata.bottomsquaretween.getValue());

	if (menutransitiondata.uppersquaretween.isfinished() && menutransitiondata.bottomsquaretween.isfinished()) {
		std::string newmenu = menutransitiondata.newmenu;

		if (menutransitiondata.actually_finished) {
			if (newmenu != "game") {
				currentmenu = newmenu;
				currentstate = STATES::MENU;
				menuscreens[currentmenu]->start();
			}
			else {
				currentstate = STATES::MAP;
				p.setCanMove(true);
			}
			menutransitiondata.actually_finished = false;
			menutransitiondata.uppersquaretween = Tweening<sf::Vector2f>(sf::Vector2f(in->getScreenSize().x, 0), sf::Vector2f(in->getScreenSize().x, in->getScreenSize().y / 2), 0.5f);
			menutransitiondata.bottomsquaretween = Tweening<sf::Vector2f>(sf::Vector2f(in->getScreenSize().x, 0), sf::Vector2f(in->getScreenSize().x, -in->getScreenSize().y / 2), 0.5f);
		}
		else {
			currentmenu = newmenu;
			menutransitiondata.uppersquaretween = Tweening<sf::Vector2f>(sf::Vector2f(in->getScreenSize().x, in->getScreenSize().y / 2), sf::Vector2f(in->getScreenSize().x, 0), 0.5f);
			menutransitiondata.bottomsquaretween = Tweening<sf::Vector2f>(sf::Vector2f(in->getScreenSize().x, -in->getScreenSize().y / 2), sf::Vector2f(in->getScreenSize().x, 0), 0.5f);
			menutransitiondata.actually_finished = true;
			setMenuTransitionOffset(newmenu);
		}
	}
}

void RoomManager::setMenuTransitionOffset(std::string menu) {
	sf::View v = w->getView();
	v.setCenter(in->getScreenSize()/2.f);
	sf::Vector2f offsetU = sf::Vector2f();
	sf::Vector2f offsetB = sf::Vector2f(0, in->getScreenSize().y);
	if (menu == "game") {
		getCurrentRoom()->update(0);
		v = getCurrentRoom()->getMainCamera();
		offsetU = getCurrentRoom()->getCameraTopLeft();
		offsetB += offsetU;
	}
	w->setView(v);
	menutransitiondata.uppersquare.setPosition(offsetU);
	menutransitiondata.bottomsquare.setPosition(offsetB);
}

void RoomManager::drawMenuTransition() {
	std::cout << menutransitiondata.uppersquare.getPosition().x << " " <<
		menutransitiondata.uppersquare.getPosition().y << " " <<
		menutransitiondata.uppersquare.getSize().x << " " <<
		menutransitiondata.uppersquare.getSize().y << "\n";
	w->draw(menutransitiondata.uppersquare);
	w->draw(menutransitiondata.bottomsquare);
}