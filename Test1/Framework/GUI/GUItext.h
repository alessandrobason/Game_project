#pragma once
#include "GUIelement.h"

class GUItext : public GUIelement {
public:
	GUItext(const GUItext& copy) : GUIelement(copy) {}
	GUItext(GUIelement* p) { parent = p; }

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

		boxrect = (sf::IntRect)text.getLocalBounds();

		alignElement();
		update();
	}

	void update() {
		text.setPosition(sf::Vector2f(boxrect.left, boxrect.top));
	}

	void draw(sf::RenderWindow* w) {
		sf::RectangleShape aaa;
		aaa.setPosition(getPosition());
		aaa.setSize(sf::Vector2f(boxrect.width, boxrect.height));
		aaa.setFillColor(sf::Color::Transparent);
		aaa.setOutlineColor(sf::Color::Red);
		aaa.setOutlineThickness(1.f);
		w->draw(text);
		//w->draw(aaa);
	}

	int getCharacterSize() { return text.getCharacterSize(); }
	void setCharacterSize(int s) { 
		text.setCharacterSize(s); 
		sf::Texture& texture = const_cast<sf::Texture&>(font->getTexture(s));
		texture.setSmooth(false);
		std::cout << "size: " << s << "\n";
	}

protected:
	//GUIelement* parent = nullptr;
	sf::Text text;
	sf::Font* font = nullptr;
};