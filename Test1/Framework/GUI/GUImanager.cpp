#include "GUImanager.h"
#include "../RoomManager.h"

GUImanager::GUImanager(sf::RenderWindow* window, InputHandler* input, RoomManager* rm) {
	in = input;
	w = window;
	roommanager = rm;

	if (!roommanager->textures["guibox"].loadFromFile("Assets/GUI/box.png")) {
		assert("couldn't load guibox texture");
	}
	guibox.setTexture(&roommanager->textures["guibox"]);
	guibox.setRect(sf::IntRect(0, 0, 124, 124));
	guibox.load();
}