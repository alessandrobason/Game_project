#include "Circle_room.h"
#include <iostream>


Circle_room::Circle_room(sf::RenderWindow* window, InputHandler* input, std::string fold, std::string config_path) : Room(fold, config_path, input) {
	w = window;
	w->setView(main_camera);
	/*
	if (!shader.loadFromFile("Shaders/grayscale.frag", sf::Shader::Fragment)) {
		std::cout << "Failed to load fragment shader\n";
	}
	shader.setUniform("texture", sf::Shader::CurrentTexture);
	*/
}

Circle_room::~Circle_room(){}

void Circle_room::handle_input() {
	if (in->isKeyDown(in->KEY_UP)) {
		main_camera.move(sf::Vector2f(0, -0.5f));
		w->setView(main_camera);
	}
	if (in->isKeyDown(in->KEY_RIGHT)) {
		main_camera.move(sf::Vector2f(0.5f, 0));
		w->setView(main_camera);
	}
	if (in->isKeyDown(in->KEY_DOWN)) {
		main_camera.move(sf::Vector2f(0, 0.5f));
		w->setView(main_camera);
	}
	if (in->isKeyDown(in->KEY_LEFT)) {
		main_camera.move(sf::Vector2f(-0.5f, 0));
		w->setView(main_camera);
	}
}

void Circle_room::update() {

}

void Circle_room::draw() {
	w->clear();

	w->draw(tilemap, &shader);

	w->display();
}

/*
void Circle_room::load() {

	std::string tileset_json;
	std::string tileset_file;

	const std::string ASSETS = "Assets/";

	std::cout << "Loading json file\n";

	// get the json file for the map
	std::string map = config->doc["map"].str;
	// load the json file for the map
	JSONparser* tilemap_json = new JSONparser(FOLDER + map);

	// load tilemap data
	tilemap_data.numb_layers = tilemap_json->doc["layers"].arr.size();
	tilemap_data.tileSize.x = tilemap_json->doc["tilewidth"].i;
	tilemap_data.tileSize.y = tilemap_json->doc["tileheight"].i;
	tilemap_data.h = tilemap_json->doc["height"].i;
	tilemap_data.w = tilemap_json->doc["width"].i;

	std::cout << "Map json file: " << map << "\n";
	std::cout << "Map height: " << tilemap_data.h << "\n";
	std::cout << "Map width: " << tilemap_data.w << "\n";
	std::cout << "Number of layers: " << tilemap_data.numb_layers << "\n";
	
	// LOAD LAYERS DATA //
	for (int i = 0; i < tilemap_data.numb_layers; i++) {
		std::cout << "Layer " << i + 1 << ": \n";
		std::cout << "\tName: " << tilemap_json->doc["layers"].arr[i].obj["name"].str << "\n";

		for (int j = 0; j < tilemap_json->doc["layers"].arr[i].obj["data"].arr.size(); j++) {
			int t = tilemap_json->doc["layers"].arr[i].obj["data"].arr[j].i;
			t--;
			layers.push_back(t);
		}
	}

	// load tileset json
	tileset_json = ASSETS + tilemap_json->doc["tilesets"].arr[0].obj["source"].str;
	JSONparser* ts_json = new JSONparser(tileset_json);

	tileset_file = ASSETS + ts_json->doc["image"].str;

	std::cout << "Tileset json file: " << tileset_json << "\n";
	std::cout << "Tileset image file: " << tileset_file << "\n";

	tilemap.load(tileset_file, layers, tilemap_data);
	
	delete tilemap_json;
	delete ts_json;
	tilemap_json = NULL;
	ts_json = NULL;
}
*/