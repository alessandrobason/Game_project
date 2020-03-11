#pragma once
#include "GUIelement.h"

class GUItext {
public:
	GUItext(GUIelement* p) { parent = p; }

	void setFont(sf::Font* f) { 
		font = f; 
		text.setFont(*font);
	}

	void load(std::string t, unsigned size, sf::Color color = sf::Color::White) {
		if (font == nullptr) {
			std::cout << "Font is not loaded\n";
			abort();
		}

		sf::Texture& texture = const_cast<sf::Texture&>(font->getTexture(size));
		texture.setSmooth(false);

		text.setString(t);
		text.setCharacterSize(size);
		text.setFillColor(color);
	}

	void update() {
		text.setPosition(sf::Vector2f(parent->getRect().left, parent->getRect().top));
	}

	void draw(sf::RenderWindow* w) {
		w->draw(text);
	}

	int getCharacterSize() { return text.getCharacterSize(); }
	void setCharacterSize(int s) { 
		text.setCharacterSize(s); 
		sf::Texture& texture = const_cast<sf::Texture&>(font->getTexture(s));
		texture.setSmooth(false);
		std::cout << "size: " << s << "\n";
	}

protected:
	GUIelement* parent = nullptr;
	sf::Text text;
	sf::Font* font = nullptr;
};