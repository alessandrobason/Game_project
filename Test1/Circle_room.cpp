#include "Circle_room.h"


Circle_room::Circle_room(sf::RenderWindow* window, InputHandler* input, std::string fold, std::string config_path) : Room(fold, config_path, input) {
	w = window;
	w->setView(main_camera);

	p = new Player(in, ASSETS + "link.png", sf::Vector2f(10, 10));
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
	p->Update(dt);
	sf::Vector2f playerSize = sf::Vector2f(p->sprite.getLocalBounds().width/2, p->sprite.getLocalBounds().height/2);
	main_camera.setCenter(p->sprite.getPosition() + playerSize);
	w->setView(main_camera);
}

void Circle_room::draw() {
	w->clear();

	//w->draw(tilemap, &shader);
	tilemap.drawUnder();
	w->draw(p->sprite);
	tilemap.drawOver();

	w->display();
}