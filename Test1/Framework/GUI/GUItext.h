#pragma once
#include "GUIelement.h"

class GUItext : public GUIelement {
public:
	GUItext() {}
	GUItext(const GUItext& copy) : GUIelement(copy) {
		text = copy.text;
		font = copy.font;
	}

	void setFont(sf::Font* f) { 
		font = f; 
		text.setFont(*font);
	}

	void setText(std::string t) { text.setString(t); }
	void setCharacterSize(unsigned s) { text.setCharacterSize(s); }

	void load(sf::Color color = sf::Color::White) {
		if (font == nullptr) {
			std::cout << "Font is not loaded\n";
			abort();
		}

		text.setFillColor(color);

		sf::Texture& texture = const_cast<sf::Texture&>(font->getTexture(text.getCharacterSize()));
		texture.setSmooth(false);

		boxrect = text.getLocalBounds();
		std::cout << "Loading text: " << text.getString().toAnsiString() << "\n";
		alignElement();
		update();
	}

	void update() {
		text.setPosition(sf::Vector2f(boxrect.left, boxrect.top));
	}

	void draw(sf::RenderWindow* w) override {
		w->draw(text);
	}

	void setPosition(sf::Vector2f pos) override {
		GUIelement::setPosition(pos);
		update();
	}

	int getCharacterSize() { return text.getCharacterSize(); }
	
	void setCharacterSize(int s) { 
		text.setCharacterSize(s); 
		sf::Texture& texture = const_cast<sf::Texture&>(font->getTexture(s));
		texture.setSmooth(false);
		std::cout << "size: " << s << "\n";
	}

protected:
	sf::Text text;
	sf::Font* font = nullptr;
};