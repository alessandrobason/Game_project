#include "GUImanager.h"
#include "../Tweening.h"
#include "../RoomManager.h"

GUImanager::GUImanager(sf::RenderWindow* window, InputHandler* input, RoomManager* rm) {
	GUIin = input;
	GUIw = window;
	GUIroommanager = rm;
	/*
	std::vector<textureload> texturestoload;
	std::vector<fontload> fontstoload;

	texturestoload.push_back({ "testanimation", "GUI/testanimation.png" });
	loadTextures(texturestoload);

	GUIpanel mainpanel;
	mainpanel.setRect(sf::FloatRect(0, 0, in->getScreenSize().x, in->getScreenSize().y));
	mainpanel.setBackgroundColor(sf::Color::Black);
	mainpanel.load();

	GUIsplashscreen splashscreen;
	splashscreen.setParent(&mainpanel);
	splashscreen.setTexture(&roommanager->textures["testanimation"]);
	splashscreen.setRect(sf::FloatRect(0, 0, 64, 64));
	splashscreen.setAlign(GUIelement::ALIGN::CENTER, GUIelement::ALIGN::CENTER);
	splashscreen.load();

	elements.push_back(new GUIpanel(mainpanel));
	elements.push_back(new GUIsplashscreen(splashscreen));

	////////////////////
	

	//GUIbox guibox;
	//guibox.setTexture(&roommanager->textures["guibox"]);
	//guibox.setRect(sf::IntRect(0, 0, in->getScreenSize().x, in->getScreenSize().y));
	//guibox.setTileSize(16);
	//guibox.load();
	
	//////////////////////////////////////////
	
	struct oldpointerstruct { GUIelement* op; int pos; };

	std::vector<oldpointerstruct> oldpointers;

	// IF PARENTS STOP WORKING UNCOMMENT THIS
	//for (size_t i = 0; i < elements.size(); i++) {
	//	oldpointers.push_back({ elements[i]->oldpointer, (int)i });
	//}

	// fix parent pointers
	for (size_t i = 0; i < elements.size(); i++) {
		oldpointers.push_back({ elements[i]->oldpointer, (int)i });
		if (elements[i]->getParent() == nullptr) continue;
		for (size_t j = 0; j < oldpointers.size(); j++) {
			if (elements[i]->getParent() == oldpointers[j].op) {
				elements[i]->setParent(elements[oldpointers[j].pos]);
				break;
			}
		}
	}
	*/
}

void GUImanager::loadTextures(std::vector<textureload> texturenames) {
	std::map<std::string, sf::Texture>::iterator it;
	for (size_t i = 0; i < texturenames.size(); i++) {
		it = GUIroommanager->textures.find(texturenames[i].n);
		if (it != GUIroommanager->textures.end()) {
			std::cout << "texture " << texturenames[i].n << " already loaded\n";
			continue;
		}
		// as long as the texture is not already loaded
		if (!GUIroommanager->textures[texturenames[i].n].loadFromFile("Assets/" + texturenames[i].f)) {
			std::cout << "couldn't load " << texturenames[i].n << " texture\n";
			abort();
		}
		std::cout << "Loading " << texturenames[i].n << "\n";
	}
}

void GUImanager::loadFonts(std::vector<fontload> fontnames) {
	std::map<std::string, sf::Font>::iterator it;
	for (size_t i = 0; i < fontnames.size(); i++) {
		it = GUIroommanager->fonts.find(fontnames[i].n);
		if (it != GUIroommanager->fonts.end()) {
			std::cout << "texture " << fontnames[i].n << " already loaded\n";
			continue;
		}
		// as long as the texture is not already loaded
		if (!GUIroommanager->fonts[fontnames[i].n].loadFromFile("Assets/" + fontnames[i].f)) {
			std::cout << "couldn't load " << fontnames[i].n << " texture\n";
			abort();
		}
		std::cout << "Loading " << fontnames[i].n << "\n";
	}
}

void GUImanager::handleInput(float dt) {
	sf::Vector2f mousepos = GUIw->mapPixelToCoords(GUIin->getMouseRelative());
	bool clicked = GUIin->isMouseLDown();

	for (size_t i = 0; i < elements.size(); i++) {
		if (elements[i]->getRect().contains(mousepos) && clicked) {
			elements[i]->changeControlState(GUIelement::CONTROL_STATES::CLICKDOWN);
		}
		else if (elements[i]->getRect().contains(mousepos) && !clicked && wasmousedown) {
			elements[i]->changeControlState(GUIelement::CONTROL_STATES::CLICKUP);
		}
		else if (elements[i]->getRect().contains(mousepos) && !clicked && !wasmousedown) {
			elements[i]->changeControlState(GUIelement::CONTROL_STATES::HOVER);
		}
		else {
			elements[i]->changeControlState(GUIelement::CONTROL_STATES::NONE);
		}
	}

	wasmousedown = clicked;
}

void GUImanager::update(float dt) {
	for (size_t i = 0; i < elements.size(); i++) {
		elements[i]->update(dt);
		if (elements[i]->isdead()) {
			//elements.erase(elements.begin() + i);
			//i--;
		}
	}
}

void GUImanager::draw() {
	for (size_t i = 0; i < elements.size(); i++) {
		elements[i]->draw(GUIw);
	}
}
