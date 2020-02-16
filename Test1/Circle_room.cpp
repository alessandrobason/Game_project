#include "Circle_room.h"

Circle_room::Circle_room(sf::RenderWindow* window, InputHandler* input, std::string fold, std::string config_path) : Room(fold, config_path, input) {
	w = window;
	w->setView(main_camera);

	p = new Player(in, this, ASSETS + "link.png", sf::Vector2f(50, 50));
	p->setSpeed(100.f);
	tilemap.setWindow(w);

	Room::load();

	sceneObjects.push_back(p);
	for (size_t i = 0; i < 3; i++) {
		sceneObjects.push_back(new Tree(&textures["tileset"], in, sf::Vector2f(16 * i * 2, 16 * i * 3)));
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

	std::cout << "AAAAAA: " << sceneColliders.size() << "\n";
	
	/*
	if (!shader.loadFromFile("Shaders/grayscale.frag", sf::Shader::Fragment)) {
		std::cout << "Failed to load fragment shader\n";
	}
	shader.setUniform("texture", sf::Shader::CurrentTexture);
	*/
}

Circle_room::~Circle_room(){}

void Circle_room::handleInput(float dt) {
	if (in->isKeyPressed(sf::Keyboard::F1)) isdebug = !isdebug;
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->handleInput(dt);
	}
}

void Circle_room::update(float dt) {
	// OTHER GAMEOBJECTS UPDATE
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->update(dt);
	}
	// PLAYER UPDATE

	for (size_t i = 0; i < sceneColliders.size(); i++) {
		sf::FloatRect rect = sceneColliders[i]->rect;
		//std::cout << i << "\n";
		if (p->collider.Check_Collision(sceneColliders[i]->rect)) {
			sf::Vector2f revVel = p->collider.getCollisionSide(sceneColliders[i]->rect, p->oldVel);
			//p->move(revVel);
			sceneColliders[i]->setDebugColor(sf::Color::Blue);
		}
		else {
			sceneColliders[i]->setDebugColor(sf::Color::Red);
		}
	}

	/*
	for (size_t i = 0; i < tilemap.collisions.size(); i++) {
		sf::FloatRect rect = tilemap.collisions[i].rect;
		if (p->collider.Check_Collision(rect)) {
			sf::Vector2f revVel = p->collider.getCollisionSide(rect, p->oldVel);
			p->move(revVel);

			std::cout << "p->left " << p->collider.rect.left << "\n";
			std::cout << "p->top " << p->collider.rect.top<< "\n";
			std::cout << "p->width " << p->collider.rect.width << "\n";
			std::cout << "p->height " << p->collider.rect.height << "\n";

			std::cout << "col->left " << rect.left << "\n";
			std::cout << "col->top " << rect.top << "\n";
			std::cout << "col->width " << rect.width << "\n";
			std::cout << "col->height " << rect.height << "\n";

			tilemap.collisionShapes[i].setOutlineColor(sf::Color::Blue);
		}
		else {
			tilemap.collisionShapes[i].setOutlineColor(sf::Color::Red);
		}
	}
	*/
	
	sf::Vector2f playerSize = sf::Vector2f(p->getSprite()->getLocalBounds().width/2, p->getSprite()->getLocalBounds().height/2);
	main_camera.setCenter(p->getSprite()->getPosition() + playerSize);
	w->setView(main_camera);

}

void Circle_room::draw() {
	w->clear();

	sortGameObjects();
	tilemap.drawUnder();
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->draw(w);
	}
	if (isdebug) {
		for (size_t i = 0; i < sceneObjects.size(); i++) {
			sceneObjects[i]->drawDebug(w);
		}
		//tilemap.drawDebug();
	}
	tilemap.drawOver();

	w->display();
}