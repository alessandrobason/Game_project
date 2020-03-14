#include "Map_room.h"
#include "Framework/RoomManager.h"

Map_room::Map_room(RoomManager* rm, sf::RenderWindow* window, InputHandler* input, std::string fold) : Room(fold, window, input, rm) {
	layers = &roommanager->tilemapData[fold].layers;
	tilemap_data = &roommanager->tilemapData[fold].tilemap_data;
	tilemap = &roommanager->tilemapData[fold].tilemap;
	shader = &roommanager->tilemapData[fold].shader;
}

Map_room::~Map_room() {
	delete layers;
	delete tilemap_data;
	delete tilemap;
	delete shader;
	layers = nullptr;
	tilemap_data = nullptr;
	tilemap = nullptr;
	shader = nullptr;
}

void Map_room::load(sf::Vector2f offs) {
	offset = offs;
	Room::load(offset);
	if (roommanager->tilemapData[FOLDER].loaded) return;
	std::cout << "Loading: " << FOLDER << "\n";
	std::string tileset_json;
	std::string tileset_file;

	const std::string ASSETS = "Assets/";

	std::cout << "Loading json file\n";
	// get the json file for the map
	std::string map = config->doc["map"].str;
	// load the json file for the map
	tilemap_json = new JSONparser("Rooms/" + FOLDER + "/" + map);

	// load tileset json
	tileset_json = tilemap_json->doc["tilesets"].arr[0].obj["source"].str;
	JSONparser* ts_json = new JSONparser(tileset_json);

	std::vector<Tilemap::animated_tiles_data> animatedTilesData;

	// load tilemap data
	tilemap_data->numb_layers = (int)tilemap_json->doc["layers"].arr.size();
	tilemap_data->tileSize.x = tilemap_json->doc["tilewidth"].i;
	tilemap_data->tileSize.y = tilemap_json->doc["tileheight"].i;
	tilemap_data->h = tilemap_json->doc["height"].i;
	tilemap_data->w = tilemap_json->doc["width"].i;

	std::cout << "Map json file: " << map << "\n";
	std::cout << "Map height: " << tilemap_data->h << "\n";
	std::cout << "Map width: " << tilemap_data->w << "\n";
	std::cout << "Number of layers: " << tilemap_data->numb_layers << "\n";

	int numb_layers = tilemap_data->numb_layers;
	std::vector<Collision> collisions;

	// LOAD LAYERS DATA //
	for (int i = 0; i < numb_layers; i++) {
		std::cout << "Layer " << i + 1 << ": \n";

		// load collisions
		if (tilemap_json->doc["layers"].arr[i].obj["type"].str == "objectgroup") {
			tilemap_data->numb_layers--;
			for (size_t k = 0; k < tilemap_json->doc["layers"].arr[i].obj["objects"].arr.size(); k++) {
				int x = offset.x + tilemap_json->doc["layers"].arr[i].obj["objects"].arr[k].obj["x"].i;
				int y = offset.y + tilemap_json->doc["layers"].arr[i].obj["objects"].arr[k].obj["y"].i;
				int w = tilemap_json->doc["layers"].arr[i].obj["objects"].arr[k].obj["width"].i;
				int h = tilemap_json->doc["layers"].arr[i].obj["objects"].arr[k].obj["height"].i;
				collisions.push_back(Collision(x, y, w, h, Collision::LAYER::ALL));
			}
			continue;
		}
		std::cout << "\tName: " << tilemap_json->doc["layers"].arr[i].obj["name"].str << "\n";

		// load tileset
		for (int j = 0; j < tilemap_json->doc["layers"].arr[i].obj["data"].arr.size(); j++) {
			int t = tilemap_json->doc["layers"].arr[i].obj["data"].arr[j].i;
			t--; // tiled starts at 1 instead of 0
			layers->push_back(t);
		}
	}

	// load tileset data

	tileset_file = ts_json->doc["image"].str;

	std::cout << "Tileset json file: " << tileset_json << "\n";
	std::cout << "Tileset image file: " << tileset_file << "\n";
	sf::Image tilesetTemp;
	//sf::Texture tilesetTemp;
	if (!tilesetTemp.loadFromFile(tileset_file)) {
		std::cout << "Couldn't load tileset from " << tileset_file << "\n";
	}

	int tilesetinimagesposition = images->size();
	images->push_back({ "tiles", tilesetTemp });

	//textures->insert(std::pair<std::string, sf::Texture>("tileset", tilesetTemp));


	std::cout << "number of tiles: " << ts_json->doc["tiles"].size << "\n";

	// loading animated tiles
	std::map<std::string, JSONparser::datatypes>::const_iterator it;
	for (auto const& x : ts_json->doc["tiles"].obj) {
		it = x.second.obj.find("animation");
		if (it != x.second.obj.end()) {
			std::vector<Tilemap::animated_tiles_data::data> temp;
			for (size_t i = 0; i < it->second.arr.size(); i++) {
				temp.push_back({ it->second.arr[i].obj.at("duration").i , it->second.arr[i].obj.at("tileid").i });
			}
			animatedTilesData.push_back({ x.first, temp });
		}
	}

	std::cout << "animatedTilesData: " << animatedTilesData.size() << "\n";

	tilemap->load(&isdebug, &images->at(tilesetinimagesposition).img, animatedTilesData, *layers, *tilemap_data, offset);
	tilemap->loadCollisions(collisions);

	delete ts_json;
	ts_json = nullptr;

	*isloaded = true;

	setBounds(offset);

	tilemap->setWindow(w);
	
	// load objects into array
	std::cout << "size: " << sceneObjects.size() << "\n";
	
	for (size_t i = 0; i < tilemap_json->doc["enemies"].arr.size(); i++) {
		std::string name = tilemap_json->doc["enemies"].arr[i].obj["name"].str;
		sf::Vector2f position = offset;
		position.x += tilemap_json->doc["enemies"].arr[i].obj["position"].obj["x"].i;
		position.y += tilemap_json->doc["enemies"].arr[i].obj["position"].obj["y"].i;
		
		if (name == "octorock") {
			sceneObjects.push_back(new Octorock(roommanager->getEnemyCopy(name)));
		}
		sceneObjects.back()->setPosition(position);
		std::cout << "##### LOADING ENEMIES #####\n";
		std::cout << "name: " << name << "\n";
		std::cout << "position: " << position.x << " " << position.y << "\n";
	}
	
	// load collisions
	for (size_t i = 0; i < tilemap->collisions.size(); i++) {
		sceneColliders.push_back(&tilemap->collisions[i]);
	}

	for (size_t i = 1; i < sceneObjects.size(); i++) {
		sceneColliders.push_back(&sceneObjects[i]->collider);
	}

	sf::VertexArray over = tilemap->getOver();

	/*
	// load shaders
	if (!shader.loadFromFile("Shaders/grayscale.frag", sf::Shader::Fragment)) {
		std::cout << "Failed to load fragment shader\n";
	}
	shader.setUniform("texture", sf::Shader::CurrentTexture);
	*/
}

void Map_room::setPlayer(Player* pl) {
	p = pl;
	sceneObjects.push_back(p);
	sceneColliders.push_back(&p->collider);
}

void Map_room::removePlayer() {
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		if (sceneObjects[i] == p){
			sceneObjects.erase(sceneObjects.begin() + i);
		}
	}
	p = nullptr; 
}

void Map_room::setPlayerPosition(sf::Vector2f pos) {
	p->setPosition(pos);
}

/*######################*\
||                      ||
||     HANDLE INPUT     ||
||                      ||
\*######################*/
void Map_room::handleInput(float dt) {
	if (in->isKeyPressed(sf::Keyboard::F1)) isdebug = !isdebug;
	
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->handleInput(dt);
	}
}

/*######################*\
||                      ||
||        UPDATE        ||
||                      ||
\*######################*/
void Map_room::update(float dt) {
	tilemap->animate(dt);

	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->update(dt);
	}

	for (size_t i = 0; i < sceneColliders.size(); i++) {
		sf::FloatRect rect = sceneColliders[i]->rect;
		if (p->collider.Check_Collision(rect)) {
			sf::Vector2f revVel = p->collider.getCollisionSide(rect, p->oldVel);
			p->move(revVel);
			sceneColliders[i]->setDebugColor(sf::Color::Blue);
		}
		else {
			sceneColliders[i]->setDebugColor(sf::Color::Red);
		}
	}

	// check if player is going in another map //
	for (size_t i = 0; i < 4; i++) {
		if (p->collider.Check_Collision(bounds[i])) {
			roommanager->moveRoom(static_cast<int>(i));
			return;
		}
	}

	sf::Vector2f playerSize = sf::Vector2f(p->getSprite()->getLocalBounds().width/2, p->getSprite()->getLocalBounds().height/2);
	//main_camera.setSize(w->getView().getSize());
	main_camera.setViewport(in->getView().getViewport());
	main_camera.setCenter(p->getSprite()->getPosition() + p->getLocalCenter());
	camera_top_left = main_camera.getCenter() - main_camera.getSize() / 2.f;
	camera_bottom_right = main_camera.getCenter() + main_camera.getSize() / 2.f;
	// STICK CAMERA //
	// left
	if (camera_top_left.x <= bounds[2].left + bounds[2].width) {
		main_camera.setCenter(sf::Vector2f(bounds[2].left + (main_camera.getSize().x / 2.f), main_camera.getCenter().y));
	}
	// right
	if (camera_bottom_right.x >= bounds[3].left) {
		main_camera.setCenter(sf::Vector2f(bounds[3].left - (main_camera.getSize().x / 2.f), main_camera.getCenter().y));
	}
	// top
	if (camera_top_left.y <= bounds[0].top) {
		main_camera.setCenter(sf::Vector2f(main_camera.getCenter().x , bounds[0].top + main_camera.getSize().y / 2.f));
	}
	// bottom
	if (camera_bottom_right.y >= bounds[1].top + bounds[1].height) {
		main_camera.setCenter(sf::Vector2f(main_camera.getCenter().x, bounds[1].top - (main_camera.getSize().y / 2.f)));
	}
	// update top_left of the camera for player
	camera_top_left = main_camera.getCenter() - main_camera.getSize() / 2.f;
	w->setView(main_camera);
	freeMemory();
	//cullGameObjects();
	//std::cout << "->" << main_camera.getCenter().x << " " << main_camera.getCenter().x << "\n";
}

/*######################*\
||                      ||
||         DRAW         ||
||                      ||
\*######################*/
void Map_room::draw() {
	sortGameObjects();
	tilemap->drawUnder();
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->draw();
	}
	
	tilemap->drawOver();

	// draw debug
	if (isdebug) {
		p->drawDebug();
		for (size_t i = 0; i < sceneColliders.size(); i++) {
			sceneColliders[i]->drawDebug(w);
		}
	}

}

void Map_room::removeObject(GameObject* g) {
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		if (sceneObjects[i] == g) {
			destroyedSceneObjects.push_back(sceneObjects[i]);
			sceneObjects[i] = sceneObjects.back();
			sceneObjects.resize(sceneObjects.size() - 1);
			break;
		}
	}
	for (size_t i = 0; i < sceneColliders.size(); i++) {
		if (sceneColliders[i] == &g->collider) {
			sceneColliders[i] = sceneColliders.back();
			sceneColliders.resize(sceneColliders.size() - 1);
			break;
		}
	}
}

void Map_room::freeMemory() {
	for (size_t i = 0; i < destroyedSceneObjects.size(); i++) {
		delete destroyedSceneObjects[i];
	}
	destroyedSceneObjects.clear();
}

void Map_room::sortGameObjects() {
	std::sort(sceneObjects.begin(), sceneObjects.end(), gameobjectsorting());
}

void Map_room::cullGameObjects() {
	sf::Vector2f size = main_camera.getSize();
	sf::Vector2f pos = main_camera.getCenter();
	sf::FloatRect rect = sf::FloatRect(pos.x - (size.x / 2), pos.y - (size.y / 2), size.x, size.y);

	if (isdebug) {
		sf::RectangleShape r;
		r.setSize(size);
		r.setPosition(sf::Vector2f(pos.x - (size.x / 2), pos.y - (size.y / 2)));
		r.setFillColor(sf::Color::Transparent);
		r.setOutlineColor(sf::Color::Green);
		r.setOutlineThickness(1.0f);
		w->draw(r);
	}

	size_t invisibleObjectSize = invisibleSceneObjects.size();
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		if (!rect.intersects(sceneObjects[i]->getSprite()->getGlobalBounds())) {
			if (sceneObjects[i] == p) continue;
			invisibleSceneObjects.push_back(sceneObjects[i]);
			sceneObjects[i] = sceneObjects[sceneObjects.size() - 1];
			sceneObjects.resize(sceneObjects.size() - 1);
		}
	}

	for (size_t i = 0; i < invisibleObjectSize; i++) {
		if (rect.intersects(invisibleSceneObjects[i]->getSprite()->getGlobalBounds())) {
			sceneObjects.push_back(invisibleSceneObjects[i]);
			invisibleSceneObjects[i] = invisibleSceneObjects[invisibleSceneObjects.size() - 1];
			invisibleSceneObjects.resize(invisibleSceneObjects.size() - 1);
			invisibleObjectSize--;
		}
	}

	size_t invisibleCollidersSize = invisibleSceneColliders.size();
	for (size_t i = 0; i < sceneColliders.size(); i++) {
		if (!rect.intersects(sceneColliders[i]->rect) || !sceneColliders[i]->isEnabled()) {
			invisibleSceneColliders.push_back(sceneColliders[i]);
			sceneColliders[i] = sceneColliders[sceneColliders.size() - 1];
			sceneColliders.resize(sceneColliders.size() - 1);
		}
	}

	for (size_t i = 0; i < invisibleCollidersSize; i++) {
		if (rect.intersects(invisibleSceneColliders[i]->rect) && invisibleSceneColliders[i]->isEnabled()) {
			sceneColliders.push_back(invisibleSceneColliders[i]);
			invisibleSceneColliders[i] = invisibleSceneColliders[invisibleSceneColliders.size() - 1];
			invisibleSceneColliders.resize(invisibleSceneColliders.size() - 1);
			invisibleCollidersSize--;
		}
	}
}

sf::Vector2f Map_room::moveRoom(sf::Transform t) {
	sf::RenderStates tilemapstate = tilemap->getStates();
	tilemapstate.transform = t;
	tilemap->setStates(tilemapstate);
	return tilemap->getPosition();
}

void Map_room::setBounds(sf::Vector2f offset) {
	this->offset = offset;
	bounds[0] = sf::FloatRect(offset.x						 , offset.y						  , static_cast<float>(roommanager->MAPSIZE), 0.1f); //top
	bounds[1] = sf::FloatRect(offset.x						 , offset.y + roommanager->MAPSIZE, static_cast<float>(roommanager->MAPSIZE), 0.1f); //bottom
	bounds[2] = sf::FloatRect(offset.x						 , offset.y						  , 0.1f									, static_cast<float>(roommanager->MAPSIZE)); //left
	bounds[3] = sf::FloatRect(offset.x + roommanager->MAPSIZE, offset.y						  , 0.1f									, static_cast<float>(roommanager->MAPSIZE)); //right
}