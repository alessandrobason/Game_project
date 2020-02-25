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
	sf::Vector2f tilemapposition = offset;
	sf::Vector2f positionsigns = sf::Vector2f(1, 1);
	if (tilemapposition.x < 0) positionsigns.x = -1;
	if (tilemapposition.y < 0) positionsigns.y = -1;

	std::cout << "tilemapposition: " << tilemapposition.x << " " << tilemapposition.y << "\n";
	bounds[0] = sf::FloatRect(tilemapposition.x						  , tilemapposition.y					    , roomManager->MAPSIZE, 0.1); //top
	bounds[1] = sf::FloatRect(tilemapposition.x						  , tilemapposition.y + roomManager->MAPSIZE, roomManager->MAPSIZE, 0.1); //bottom
	bounds[2] = sf::FloatRect(tilemapposition.x						  , tilemapposition.y					    , 0.1				  , roomManager->MAPSIZE); //left
	bounds[3] = sf::FloatRect(tilemapposition.x + roomManager->MAPSIZE, tilemapposition.y						, 0.1				  , roomManager->MAPSIZE); //right
	
	std::cout << bounds[0].left << " " << bounds[0].top << " " << bounds[0].width << " " << bounds[0].height << "\n";
	std::cout << bounds[1].left << " " << bounds[1].top << " " << bounds[1].width << " " << bounds[1].height << "\n";
	std::cout << bounds[2].left << " " << bounds[2].top << " " << bounds[2].width << " " << bounds[2].height << "\n";
	std::cout << bounds[3].left << " " << bounds[3].top << " " << bounds[3].width << " " << bounds[3].height << "\n";
	
	tilemap->setWindow(w);

	//p = new Player(in, this, sf::Vector2f());

	// load objects into array
	std::cout << "size: " << sceneObjects.size() << "\n";
	for (size_t i = 0; i < 5; i++) {
		//sceneObjects.push_back(new Tree(&textures["tileset"], in, sf::Vector2f(16 * i * 2, 16 * i * 2)));
	}

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

void Forest_room::handleInput(float dt) {
	if (in->isKeyPressed(sf::Keyboard::F1)) isdebug = !isdebug;
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->handleInput(dt);
	}
}

void Forest_room::update(float dt) {
	tilemap->animate(dt);

	// OTHER GAMEOBJECTS UPDATE
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->update(dt);
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
			roomManager->moveRoom(i);
			return;
		}
	}

	sf::Vector2f playerSize = sf::Vector2f(p->getSprite()->getLocalBounds().width/2, p->getSprite()->getLocalBounds().height/2);
	main_camera.setCenter(p->getSprite()->getPosition() + playerSize);
	sf::Vector2f top_left_camera = main_camera.getCenter() - main_camera.getSize() / 2.f;
	sf::Vector2f bottom_right_camera = main_camera.getCenter() + main_camera.getSize() / 2.f;
	// STICK CAMERA //
	// left
	if (top_left_camera.x <= bounds[2].left + bounds[2].width) {
		main_camera.setCenter(sf::Vector2f(main_camera.getSize().x / 2.f, main_camera.getCenter().y));
	}
	// right
	if (bottom_right_camera.x >= bounds[3].left) {
		std::cout << bounds[3].left << " " << main_camera.getSize().x / 2.f << " " << bounds[3].left - (main_camera.getSize().x / 2.f) << "\n";
		main_camera.setCenter(sf::Vector2f(bounds[3].left - (main_camera.getSize().x / 2.f), main_camera.getCenter().y));
	}
	// top
	if (top_left_camera.y <= bounds[0].top) {
		main_camera.setCenter(sf::Vector2f(main_camera.getCenter().x , main_camera.getSize().y / 2.f));
	}
	// bottom
	if (bottom_right_camera.y >= bounds[1].top + bounds[1].height) {
		main_camera.setCenter(sf::Vector2f(main_camera.getCenter().x, roomManager->MAPSIZE - (main_camera.getSize().y / 2.f)));
	}
	w->setView(main_camera);
	cullGameObjects();
	//std::cout << "->" << main_camera.getCenter().x << " " << main_camera.getCenter().x << "\n";
}

void Forest_room::draw() {
	sortGameObjects();
	tilemap->drawUnder();
	for (size_t i = 0; i < sceneObjects.size(); i++) {
		sceneObjects[i]->draw(w);
	}
	
	tilemap->drawOver();

	// draw debug
	if (isdebug) {
		p->drawDebug(w);
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
	if (isdebug) {
		//size.x /= 2;
		//size.y /= 2;
	}
	sf::Vector2f pos = main_camera.getCenter();
	sf::FloatRect rect = sf::FloatRect(pos.x - (size.x / 2), pos.y - (size.y / 2), size.x, size.y);

	sf::RectangleShape r;
	r.setSize(size);
	r.setPosition(sf::Vector2f(pos.x - (size.x / 2), pos.y - (size.y / 2)));
	r.setFillColor(sf::Color::Transparent);
	r.setOutlineColor(sf::Color::Green);
	r.setOutlineThickness(1.0f);
	if (isdebug) w->draw(r);

	//std::cout << "position: " << pos.x << " " << pos.y << "\n";
	//std::cout << rect.left << " " << rect.top << " " << rect.width << " " << rect.height << "\n";
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
		if (!rect.intersects(sceneColliders[i]->rect)) {
			invisibleSceneColliders.push_back(sceneColliders[i]);
			sceneColliders[i] = sceneColliders[sceneColliders.size() - 1];
			sceneColliders.resize(sceneColliders.size() - 1);
		}
	}

	for (size_t i = 0; i < invisibleCollidersSize; i++) {
		if (rect.intersects(invisibleSceneColliders[i]->rect)) {
			sceneColliders.push_back(invisibleSceneColliders[i]);
			invisibleSceneColliders[i] = invisibleSceneColliders[invisibleSceneColliders.size() - 1];
			invisibleSceneColliders.resize(invisibleSceneColliders.size() - 1);
			invisibleCollidersSize--;
		}
	}
}