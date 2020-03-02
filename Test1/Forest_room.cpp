#include "Forest_room.h"
#include "Framework/RoomManager.h"

Forest_room::Forest_room(RoomManager* rm, sf::RenderWindow* window, InputHandler* input, std::string fold) : Room(fold, input, rm) {
	std::cout << "1\n";
	w = window;
	std::cout << "2\n";
	w->setView(main_camera);
}

void Forest_room::load(sf::Vector2f offset) {
	Room::load(offset);
	setBounds(offset);

	tilemap->setWindow(w);
	
	// load objects into array
	std::cout << "size: " << sceneObjects.size() << "\n";
	
	//sceneObjects.push_back(new Enemy(&roomManager->textures["tileset"], in, w, offset));
	enemies.push_back(new Enemy(&roomManager->textures["tileset"], in, w, offset));

	// load collisions
	//sceneColliders.push_back(&p->collider);
	for (size_t i = 0; i < tilemap->collisions.size(); i++) {
		sceneColliders.push_back(&tilemap->collisions[i]);
		//std::cout << i << "\n";
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

void Forest_room::setPlayer(Player* pl) {
	p = pl;
	sceneObjects.push_back(p);
}

void Forest_room::removePlayer() {
	//delete p;
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		if (sceneObjects[i] == p){
			sceneObjects.erase(sceneObjects.begin() + i);
		}
	}
	p = nullptr; 
}

void Forest_room::setPlayerPosition(sf::Vector2f pos) {
	p->setPosition(pos);
}

/*######################*\
||                      ||
||     HANDLE INPUT     ||
||                      ||
\*######################*/
void Forest_room::handleInput(float dt) {
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
void Forest_room::update(float dt) {
	tilemap->animate(dt);

	// OTHER GAMEOBJECTS UPDATE
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->update(dt);
	}
	for (size_t i = 0; i < enemies.size(); i++) {
		enemies[i]->update(dt);
	}

	// PLAYER UPDATE
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
		/*
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
		*/
	}

	// check if player is going in another map //
	for (size_t i = 0; i < 4; i++) {
		if (p->collider.Check_Collision(bounds[i])) {
			roomManager->moveRoom(static_cast<int>(i));
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
	cullGameObjects();
	//std::cout << "->" << main_camera.getCenter().x << " " << main_camera.getCenter().x << "\n";
}

/*######################*\
||                      ||
||         DRAW         ||
||                      ||
\*######################*/
void Forest_room::draw() {
	sortGameObjects();
	tilemap->drawUnder();
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->draw();
	}
	for (size_t i = 0; i < enemies.size(); i++) {
		enemies[i]->draw();
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


void Forest_room::sortGameObjects() {
	std::sort(sceneObjects.begin(), sceneObjects.end(), gameobjectsorting());
}

void Forest_room::cullGameObjects() {
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

sf::Vector2f Forest_room::moveRoom(sf::Transform t) {
	sf::RenderStates tilemapstate = tilemap->getStates();
	tilemapstate.transform = t;
	tilemap->setStates(tilemapstate);
	return tilemap->getPosition();
}

void Forest_room::setBounds(sf::Vector2f offset) {
	this->offset = offset;
	bounds[0] = sf::FloatRect(offset.x						 , offset.y						  , static_cast<float>(roomManager->MAPSIZE), 0.1f); //top
	bounds[1] = sf::FloatRect(offset.x						 , offset.y + roomManager->MAPSIZE, static_cast<float>(roomManager->MAPSIZE), 0.1f); //bottom
	bounds[2] = sf::FloatRect(offset.x						 , offset.y						  , 0.1f									, static_cast<float>(roomManager->MAPSIZE)); //left
	bounds[3] = sf::FloatRect(offset.x + roomManager->MAPSIZE, offset.y						  , 0.1f									, static_cast<float>(roomManager->MAPSIZE)); //right
}