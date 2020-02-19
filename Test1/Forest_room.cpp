#include "Forest_room.h"

Forest_room::Forest_room(sf::RenderWindow* window, InputHandler* input, std::string fold, std::string config_path) : Room(fold, config_path, input) {
	w = window;
	w->setView(main_camera);

	p = new Player(in, this, ASSETS + "main character.png", sf::Vector2f(0, 0));
	p->setSpeed(100.f);
	tilemap.setWindow(w);

	Room::load();

	sceneObjects.push_back(p);
	for (size_t i = 0; i < 3; i++) {
		sceneObjects.push_back(new Tree(&textures["tileset"], in, sf::Vector2f(16 * i * 4, 16 * i * 3)));
	}

	// load collisions
	//sceneColliders.push_back(&p->collider);
	for (size_t i = 0; i < tilemap.collisions.size(); i++) {
		sceneColliders.push_back(&tilemap.collisions[i]);
		//std::cout << i << "\n";
	}
	for (size_t i = 1; i < sceneObjects.size(); i++) {
		sceneColliders.push_back(&sceneObjects[i]->collider);
	}

	sf::VertexArray over = tilemap.getOver();
	
	/*
	if (!shader.loadFromFile("Shaders/grayscale.frag", sf::Shader::Fragment)) {
		std::cout << "Failed to load fragment shader\n";
	}
	shader.setUniform("texture", sf::Shader::CurrentTexture);
	*/
}

Forest_room::~Forest_room(){}

void Forest_room::handleInput(float dt) {
	if (in->isKeyPressed(sf::Keyboard::F1)) isdebug = !isdebug;
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->handleInput(dt);
	}
}

void Forest_room::update(float dt) {
	// OTHER GAMEOBJECTS UPDATE
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->update(dt);
	}
	// PLAYER UPDATE

	for (size_t i = 0; i < sceneColliders.size(); i++) {
		sf::FloatRect rect = sceneColliders[i]->rect;
		for (size_t k = 0; k < sceneObjects.size(); k++) {
			if (sceneObjects[k]->collider.Check_Collision(rect)) {
				sf::Vector2f revVel = sceneObjects[k]->collider.getCollisionSide(rect, sceneObjects[k]->oldVel);
				sceneObjects[k]->move(revVel);
				sceneColliders[i]->setDebugColor(sf::Color::Blue);
			}
			else {
				sceneColliders[i]->setDebugColor(sf::Color::Red);
			}
		}
	}

	sf::Vector2f playerSize = sf::Vector2f(p->getSprite()->getLocalBounds().width/2, p->getSprite()->getLocalBounds().height/2);
	main_camera.setCenter(p->getSprite()->getPosition() + playerSize);
	w->setView(main_camera);

}

void Forest_room::draw() {
	w->clear();

	//std::vector<sf::VertexArray> rows;
	//sf::VertexArray over = tilemap.getOver();
	//Tilemap::tilemap_data data = tilemap.getData();

	//int width = over.getVertexCount() / data.w;
	//int positionInOver = 0;

	sortGameObjects();
	tilemap.drawUnder();
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->draw(w);
	}

	//for (size_t i = 0; i < rows.size(); i++) {
		//w->draw(rows[i], tilemap.getStates());
	//}

	tilemap.drawOver();

	// draw debug
	if (isdebug) {
		for (size_t i = 0; i < sceneObjects.size(); i++) {
			sceneObjects[i]->drawDebug(w);
		}
		tilemap.drawDebug();
	}

	w->display();
}