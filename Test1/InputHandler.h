#pragma once
#include <SFML/Graphics.hpp>

class InputHandler
{
public:

	void setKeyDown(unsigned int key) { keys[key] = true; };
	void setKeyUp(unsigned int key) { keys[key] = false; pressed_keys[key] = false;};
	bool isKeyDown(unsigned int key) { return keys[key]; };
	bool isKeyPressed(unsigned int key) {
		if (keys[key] && !pressed_keys[key]) {
			pressed_keys[key] = true;
			return true;
		}
		return false;
	}

	void setMouseX(int lx) { mouse.position.x = lx; };
	void setMouseY(int ly) { mouse.position.y = ly; };
	void setMousePosition(sf::Vector2i pos) { mouse.position = pos; };
	int getMouseX() { return mouse.position.x; };
	int getMouseY() { return mouse.position.y; };
	sf::Vector2i getMousePosition() { return mouse.position; };
	void setMouseLDown(bool down) { mouse.left = down; };
	void setMouseRDown(bool down) { mouse.right = down; };
	bool isMouseLDown() { return mouse.left; };
	bool isMouseRDown() { return mouse.right; };

	enum KEY_ENUM {
		KEY_UP = sf::Keyboard::W,
		KEY_LEFT = sf::Keyboard::A,
		KEY_RIGHT = sf::Keyboard::D,
		KEY_DOWN = sf::Keyboard::S,
		KEY_SPACE = sf::Keyboard::Space
	};

private:

	struct Mouse {
		sf::Vector2i position;
		bool left = false;
		bool right = false;
	};

	bool keys[256]{ false };
	bool pressed_keys[256]{ false };
	Mouse mouse;
};

