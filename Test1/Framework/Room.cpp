#include "Room.h"

Room::Room(std::string fold, std::string config_path, InputHandler* input){
	FOLDER = fold;
	config = new JSONparser(FOLDER + config_path);

	main_camera = sf::View(sf::FloatRect(0, 0, 16*12, 16*9));

	in = input;
}

Room::~Room(){
	delete config;
	config = NULL;
}

void Room::handleInput(float dt) {}
void Room::update(float dt) {}
void Room::draw() {}

void Room::load() {
	std::string tileset_json;
	std::string tileset_file;

	const std::string ASSETS = "Assets/";

	std::cout << "Loading json file\n";

	// get the json file for the map
	std::string map = config->doc["map"].str;
	// load the json file for the map
	JSONparser* tilemap_json = new JSONparser(FOLDER + map);

	// load tilemap data
	tilemap_data.numb_layers = (int) tilemap_json->doc["layers"].arr.size();
	tilemap_data.tileSize.x = tilemap_json->doc["tilewidth"].i;
	tilemap_data.tileSize.y = tilemap_json->doc["tileheight"].i;
	tilemap_data.h = tilemap_json->doc["height"].i;
	tilemap_data.w = tilemap_json->doc["width"].i;

	std::cout << "Map json file: " << map << "\n";
	std::cout << "Map height: " << tilemap_data.h << "\n";
	std::cout << "Map width: " << tilemap_data.w << "\n";
	std::cout << "Number of layers: " << tilemap_data.numb_layers << "\n";

	int numb_layers = tilemap_data.numb_layers;
	std::vector<Collision> collisions;

	// LOAD LAYERS DATA //
	for (int i = 0; i < numb_layers; i++) {
		std::cout << "Layer " << i + 1 << ": \n";

		if(tilemap_json->doc["layers"].arr[i].obj["type"].str == "objectgroup") {
			tilemap_data.numb_layers--;
			for (size_t k = 0; k < tilemap_json->doc["layers"].arr[i].obj["objects"].arr.size(); k++) {
				int x = tilemap_json->doc["layers"].arr[3].obj["objects"].arr[k].obj["x"].i;
				int y = tilemap_json->doc["layers"].arr[i].obj["objects"].arr[k].obj["y"].i;
				int w = tilemap_json->doc["layers"].arr[i].obj["objects"].arr[k].obj["width"].i;
				int h = tilemap_json->doc["layers"].arr[i].obj["objects"].arr[k].obj["height"].i;
				collisions.push_back(Collision(x, y, w, h));
			}
			continue;
		}
		std::cout << "\tName: " << tilemap_json->doc["layers"].arr[i].obj["name"].str << "\n";

		for (int j = 0; j < tilemap_json->doc["layers"].arr[i].obj["data"].arr.size(); j++) {
			int t = tilemap_json->doc["layers"].arr[i].obj["data"].arr[j].i;
			t--;
			layers.push_back(t);
		}
	}

	// load tileset json
	tileset_json = /*ASSETS +*/ tilemap_json->doc["tilesets"].arr[0].obj["source"].str;
	JSONparser* ts_json = new JSONparser(tileset_json);

	tileset_file = /*ASSETS +*/ ts_json->doc["image"].str;

	std::cout << "Tileset json file: " << tileset_json << "\n";
	std::cout << "Tileset image file: " << tileset_file << "\n";

	//textures["tileset"] = sf::Texture();

	if (!textures["tileset"].loadFromFile(tileset_file)) {
		std::cout << "Couldn't load tileset from " << tileset_file << "\n";
	}

	tilemap.load(&isdebug, &textures["tileset"], layers, tilemap_data);
	tilemap.loadCollisions(collisions);

	delete tilemap_json;
	delete ts_json;
	tilemap_json = NULL;
	ts_json = NULL;
}

void Room::sortGameObjects() {
	std::sort(sceneObjects.begin(), sceneObjects.end(), gameobjectsorting());
}