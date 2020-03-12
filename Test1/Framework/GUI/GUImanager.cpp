#include "GUImanager.h"
#include "../RoomManager.h"

GUImanager::GUImanager(sf::RenderWindow* window, InputHandler* input, RoomManager* rm) {
	in = input;
	w = window;
	roommanager = rm;
	std::vector<textureload> texturestoload;
	std::vector<fontload> fontstoload;

	texturestoload.push_back({"guibox", "box.png"});
	texturestoload.push_back({"guibutton", "button.png"});
	texturestoload.push_back({"guibuttonhover", "buttonhover.png"});
	texturestoload.push_back({"guibuttonpressed", "buttonpressed.png"});
	loadTextures(texturestoload);

	fontstoload.push_back({"font", "TLOZ-Links-Awakening.ttf"});
	loadFonts(fontstoload);

	
	GUIbox guibox;
	guibox.setTexture(&roommanager->textures["guibox"]);
	guibox.setRect(sf::IntRect(0, 0, in->getScreenSize().x, in->getScreenSize().y));
	guibox.setTileSize(16);
	guibox.load();
	
	GUIbutton button;
	button.setParent(&guibox);
	button.setAlign(GUIelement::ALIGN::CENTER, GUIelement::ALIGN::CENTER);
	button.setTexture(&roommanager->textures["guibutton"]);
	button.setNormalTexture(&roommanager->textures["guibutton"]);
	button.setHoverTexture(&roommanager->textures["guibuttonhover"]);
	button.setPressedTexture(&roommanager->textures["guibuttonpressed"]);
	button.setRect(sf::IntRect(0, 0, 64, 24));
	//button.setCenter(sf::Vector2f(0, 0));
	button.setTileSize(8);
	button.setText(&roommanager->fonts["font"], "CiAoBiTcH");
	button.load();
	elements.push_back(new GUIbox(guibox));
	elements.push_back(new GUIbutton(button));

	struct oldpointerstruct { GUIelement* op; int pos; };

	std::vector<oldpointerstruct> oldpointers;

	for (size_t i = 0; i < elements.size(); i++) {
		oldpointers.push_back({ elements[i]->oldpointer, (int)i });
		sf::Vector2i objpos = w->mapCoordsToPixel(sf::Vector2f(elements[i]->getRect().left, elements[i]->getRect().top));
		sf::Vector2i objsize = w->mapCoordsToPixel(sf::Vector2f(elements[i]->getRect().width, elements[i]->getRect().height));
		elementsrects.push_back(sf::IntRect(objpos, objsize));
	}

	// fix parent pointers
	for (size_t i = 0; i < elements.size(); i++) {
		if (elements[i]->getParent() == nullptr) continue;
		for (size_t j = 0; j < oldpointers.size(); j++) {
			if (elements[i]->getParent() == oldpointers[j].op) {
				elements[i]->setParent(elements[oldpointers[j].pos]);
				break;
			}
		}
	}
}

void GUImanager::loadTextures(std::vector<textureload> texturenames) {
	std::map<std::string, sf::Texture>::iterator it;
	for (size_t i = 0; i < texturenames.size(); i++) {
		it = roommanager->textures.find(texturenames[i].n);
		if (it != roommanager->textures.end()) {
			std::cout << "texture " << texturenames[i].n << " already loaded\n";
			continue;
		}
		// as long as the texture is not already loaded
		if (!roommanager->textures[texturenames[i].n].loadFromFile("Assets/GUI/" + texturenames[i].f)) {
			std::cout << "couldn't load " << texturenames[i].n << " texture\n";
			abort();
		}
		std::cout << "Loading " << texturenames[i].n << "\n";
	}
}

void GUImanager::loadFonts(std::vector<fontload> fontnames) {
	std::map<std::string, sf::Font>::iterator it;
	for (size_t i = 0; i < fontnames.size(); i++) {
		it = roommanager->fonts.find(fontnames[i].n);
		if (it != roommanager->fonts.end()) {
			std::cout << "texture " << fontnames[i].n << " already loaded\n";
			continue;
		}
		// as long as the texture is not already loaded
		if (!roommanager->fonts[fontnames[i].n].loadFromFile("Assets/" + fontnames[i].f)) {
			std::cout << "couldn't load " << fontnames[i].n << " texture\n";
			abort();
		}
		std::cout << "Loading " << fontnames[i].n << "\n";
	}
}


void GUImanager::handleInput(float dt) {
	sf::Vector2i mousepos = in->getMouseRelative();
	bool clicked = in->isMouseLDown();

	for (size_t i = 0; i < elementsrects.size(); i++) {
		if (elementsrects[i].contains(mousepos) && clicked) {
			elements[i]->changeControlState(GUIelement::CONTROL_STATES::CLICK);
		}
		else if (elementsrects[i].contains(mousepos) && !clicked) {
			elements[i]->changeControlState(GUIelement::CONTROL_STATES::HOVER);
		}
		else {
			elements[i]->changeControlState(GUIelement::CONTROL_STATES::NONE);
		}
	}
}

void GUImanager::update(float dt) {

	if (in->isKeyPressed(sf::Keyboard::Num1)) {
		elements[0]->setCharacterSize(elements[0]->getCharacterSize() + 1);
	}
	if (in->isKeyPressed(sf::Keyboard::Num2)) {
		elements[0]->setCharacterSize(elements[0]->getCharacterSize() - 1);
	}
}

void GUImanager::draw() {
	//w->clear(sf::Color::Cyan);
	for (size_t i = 0; i < elements.size(); i++) {
		elements[i]->draw(w);
	}
}
