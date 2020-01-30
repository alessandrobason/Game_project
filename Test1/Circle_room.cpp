#include "Circle_room.h"


Circle_room::Circle_room(sf::RenderWindow* window, InputHandler* input, std::string fold, std::string config_path) : Room(fold, config_path, input) {
	w = window;
	w->setView(main_camera);

	p = new Player(in, ASSETS + "link.png", sf::Vector2f(50, 50));
	tilemap.setWindow(w);
	/*
	if (!shader.loadFromFile("Shaders/grayscale.frag", sf::Shader::Fragment)) {
		std::cout << "Failed to load fragment shader\n";
	}
	shader.setUniform("texture", sf::Shader::CurrentTexture);
	*/
}

Circle_room::~Circle_room(){}

void Circle_room::handle_input() {}

void Circle_room::update(float dt) {
	sf::Vector2f vel = p->testMovement(dt);

	p->move(sf::Vector2f(vel.x, vel.y));
	for (size_t i = 0; i < tilemap.collisions.size(); i++) {
		sf::FloatRect rect = tilemap.collisions[i].rect;
		if (p->collision.Check_Collision(rect)) {
			std::cout << p->collision.rect.left << "\t" << p->collision.rect.top << "\n";
			std::cout << p->collision.rect.left+ p->collision.rect.top << "\n\n";
			std::cout << rect.left << "\t" << rect.top << "\n";
			std::cout << rect.left + rect.top << "\n\n";
			p->move(sf::Vector2f(-vel.x, -vel.y));
			tilemap.collisionShapes[i].setOutlineColor(sf::Color::Blue);
		}
		else {
			tilemap.collisionShapes[i].setOutlineColor(sf::Color::Red);
		}
	}

	/*
	p->move(sf::Vector2f(vel.x, 0));
	for (size_t i = 0; i < tilemap.collisions.size(); i++) {
		sf::FloatRect rect = tilemap.collisions[i].rect;
		if (p->collision.Check_Collision(rect)) {
			p->move(sf::Vector2f(-vel.x, 0));
			tilemap.collisionShapes[i].setOutlineColor(sf::Color::Blue);
			break;
		}
		else {
			tilemap.collisionShapes[i].setOutlineColor(sf::Color::Red);
		}
	}

	p->move(sf::Vector2f(0, vel.y));
	for (size_t i = 0; i < tilemap.collisions.size(); i++) {
		sf::FloatRect rect = tilemap.collisions[i].rect;
		if (p->collision.Check_Collision(rect)) {
			p->move(sf::Vector2f(0, -vel.y));
			tilemap.collisionShapes[i].setOutlineColor(sf::Color::Blue);
			break;
		}
		else {
			tilemap.collisionShapes[i].setOutlineColor(sf::Color::Red);
		}
	}
	*/

	
	sf::Vector2f playerSize = sf::Vector2f(p->sprite.getLocalBounds().width/2, p->sprite.getLocalBounds().height/2);
	main_camera.setCenter(p->sprite.getPosition() + playerSize);
	w->setView(main_camera);
}

void Circle_room::draw() {
	w->clear();

	//w->draw(tilemap, &shader);
	tilemap.drawUnder();
	p->draw(w);
	tilemap.drawOver();

	w->display();
}