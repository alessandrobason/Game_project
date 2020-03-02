#include "Room.h"
#include "RoomManager.h"

Room::Room(std::string fold, InputHandler* input, RoomManager* rm){
	FOLDER = fold;
	std::cout << FOLDER + "/" + FOLDER + ".json" << "\n";
	config = new JSONparser("Levels/" + FOLDER + "/" + FOLDER + ".json");

	in = input;
	roomManager = rm;

	main_camera = in->getView();

	images = &roomManager->images;
	layers = &roomManager->tilemapData[fold].layers;
	tilemap_data = &roomManager->tilemapData[fold].tilemap_data;
	tilemap = &roomManager->tilemapData[fold].tilemap;
	shader = &roomManager->tilemapData[fold].shader;
	isloaded = &roomManager->tilemapData[fold].loaded;
}

Room::~Room() {
	delete config;
	delete layers;
	delete tilemap_data;
	delete tilemap;
	delete shader;
	config = nullptr;
	layers = nullptr;
	tilemap_data = nullptr;
	tilemap = nullptr;
	shader = nullptr;
}

void Room::handleInput(float dt) {}
void Room::update(float dt) {}
void Room::draw() {}

void Room::load(sf::Vector2f offs) {
	offset = offs;
	if (roomManager->tilemapData[FOLDER].loaded) return;
	std::cout << "Loading: " << FOLDER << "\n";
	std::string tileset_json;
	std::string tileset_file;

	const std::string ASSETS = "Assets/";

	std::cout << "Loading json file\n";
	// get the json file for the map
	std::string map = config->doc["map"].str;
	// load the json file for the map
	JSONparser* tilemap_json = new JSONparser("Levels/" + FOLDER + "/" + map);

	// load tileset json
	tileset_json = tilemap_json->doc["tilesets"].arr[0].obj["source"].str;
	JSONparser* ts_json = new JSONparser(tileset_json);

	std::vector<Tilemap::animated_tiles_data> animatedTilesData;

	// load tilemap data
	tilemap_data->numb_layers = (int) tilemap_json->doc["layers"].arr.size();
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
		if(tilemap_json->doc["layers"].arr[i].obj["type"].str == "objectgroup") {
			tilemap_data->numb_layers--;
			for (size_t k = 0; k < tilemap_json->doc["layers"].arr[i].obj["objects"].arr.size(); k++) {
				int x = offset.x + tilemap_json->doc["layers"].arr[i].obj["objects"].arr[k].obj["x"].i;
				int y = offset.y + tilemap_json->doc["layers"].arr[i].obj["objects"].arr[k].obj["y"].i;
				int w = tilemap_json->doc["layers"].arr[i].obj["objects"].arr[k].obj["width"].i;
				int h = tilemap_json->doc["layers"].arr[i].obj["objects"].arr[k].obj["height"].i;
				collisions.push_back(Collision(x, y, w, h, Collision::TILE));
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
	images->push_back({"tiles", tilesetTemp});

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
			animatedTilesData.push_back({x.first, temp});
		}
	}

	std::cout << "animatedTilesData: " << animatedTilesData.size() << "\n";

	tilemap->load(&isdebug, &images->at(tilesetinimagesposition).img, animatedTilesData, *layers, *tilemap_data, offset);
	tilemap->loadCollisions(collisions);

	delete tilemap_json;
	delete ts_json;
	tilemap_json = nullptr;
	ts_json = nullptr;

	*isloaded = true;
}