#include "Circle_room.h"


Circle_room::Circle_room(sf::RenderWindow* window, InputHandler* input, std::string fold, std::string config_path) : Room(fold, config_path, input) {
	w = window;
	w->setView(main_camera);

	p = new Player(in, ASSETS + "link.png", sf::Vector2f(50, 50));
	p->setSpeed(100.f);
	tilemap.setWindow(w);
	/*
	if (!shader.loadFromFile("Shaders/grayscale.frag", sf::Shader::Fragment)) {
		std::cout << "Failed to load fragment shader\n";
	}
	shader.setUniform("texture", sf::Shader::CurrentTexture);
	*/
}

Circle_room::~Circle_room(){}

void Circle_room::handleInput(float dt) {
	p->handleInput(dt);
}

void Circle_room::update(float dt) {
	p->update(dt); 

	for (size_t i = 0; i < tilemap.collisions.size(); i++) {
		sf::FloatRect rect = tilemap.collisions[i].rect;
		if (p->collision.Check_Collision(rect)) {
			sf::Vector2f revVel = p->collision.getCollisionSide(rect, p->oldVel);
			p->move(revVel);
			tilemap.collisionShapes[i].setOutlineColor(sf::Color::Blue);
		}
		else {
			tilemap.collisionShapes[i].setOutlineColor(sf::Color::Red);
		}
	}
	
	sf::Vector2f playerSize = sf::Vector2f(p->getSprite()->getLocalBounds().width/2, p->getSprite()->getLocalBounds().height/2);
	main_camera.setCenter(p->getSprite()->getPosition() + playerSize);
	w->setView(main_camera);
}

void Circle_room::draw() {
	w->clear();

	//w->draw(tilemap, &shader);
	tilemap.drawUnder();
	p->draw(w);
	//w->draw(*p);
	tilemap.drawOver();

	w->display();
}