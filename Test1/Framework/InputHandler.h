#pragma once
#include <SFML/Graphics.hpp>

class InputHandler
{
public:
	InputHandler(sf::RenderWindow* win) { w = win; }

	void setKeyDown(unsigned int key) { if(key < 256) keys[key] = true;};
	void setKeyUp(unsigned int key) { if (key < 256) { keys[key] = false; pressed_keys[key] = false; } };
	bool isKeyDown(unsigned int key) { if (key >= 256) return false; return keys[key];};
	bool isKeyPressed(unsigned int key) {
		if (key >= 256) return false;
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
	sf::Vector2i getMouseRelative() { return sf::Mouse::getPosition(*w); }
	void setMouseLDown(bool down) { mouse.left = down; };
	void setMouseRDown(bool down) { mouse.right = down; };
	bool isMouseLDown() { return mouse.left; };
	bool isMouseRDown() { return mouse.right; };


	void resizeView(sf::Vector2u windowsize, sf::Vector2u designedsize) {
		sf::FloatRect viewport(0.f, 0.f, 1.f, 1.f);

		float screenwidth = windowsize.x / designedsize.x;
		float screenheight = windowsize.y / designedsize.y;

		if (screenwidth > screenheight) {
			viewport.width = screenheight / screenwidth;
			viewport.left = (1.f - viewport.width) / 2.f;
		}
		else if (screenwidth < screenheight) {
			viewport.height = screenwidth / screenheight;
			viewport.top = (1.f - viewport.height) / 2.f;
		}

		sf::View view(sf::FloatRect(0, 0, designedsize.x, designedsize.y));
		view.setViewport(viewport);
		windowview = view;
	}

	sf::View getView() { return windowview; }

	enum KEY_ENUM {
		KEY_UP = sf::Keyboard::W,
		KEY_LEFT = sf::Keyboard::A,
		KEY_RIGHT = sf::Keyboard::D,
		KEY_DOWN = sf::Keyboard::S,
		KEY_SPACE = sf::Keyboard::Space
	};

private:
	sf::RenderWindow* w = nullptr;

	struct Mouse {
		sf::Vector2i position;
		bool left = false;
		bool right = false;
	};

	bool keys[256]{ false };
	bool pressed_keys[256]{ false };
	Mouse mouse;

	sf::View windowview = sf::View(sf::FloatRect(0, 0, 16 * 12, 16 * 9));
};

