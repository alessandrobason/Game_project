#include "GUIbutton.h"

GUIbutton::GUIbutton(const GUIbutton& copy) : GUItiledelement(copy) {
	tilesize = copy.tilesize;
	normal = copy.normal;
	hover = copy.hover;
	pressed = copy.pressed;
	insidetext = copy.insidetext;
}

void GUIbutton::setText(sf::Font* f, std::string text) {
	insidetext.setFont(f);
	insidetext.setText(text);
	insidetext.setCharacterSize(8);
	insidetext.setAlign(ALIGN::CENTER, ALIGN::CENTER);
}

void GUIbutton::draw(sf::RenderWindow* w) {
	GUIelement::draw(w);
	insidetext.draw(w);
}

void GUIbutton::trigger() {
}

void GUIbutton::load() {
	GUItiledelement::load();
	if (states.texture == NULL) {
		std::cout << "Texture not loaded\n";
		abort();
	}

	nineslice(tilesize);
	alignElement();
	insidetext.setOffset(sf::Vector2f(0, -4));
	insidetext.load();
}

void GUIbutton::changeControlState(CONTROL_STATES newcontrol) {

	switch (newcontrol)	{
	case CONTROL_STATES::NONE:
		if (newcontrol != currentcontrol) {
			//std::cout << "Nothing the button\r";
			states.texture = normal;
			if (newcontrol != currentcontrol && currentcontrol == CONTROL_STATES::CLICK) {
				insidetext.setOffset(sf::Vector2f(0, -2));
				insidetext.updatePosition();
				insidetext.update();
			}
		}
		break;
	case CONTROL_STATES::HOVER:
		if (newcontrol != currentcontrol) {
			//std::cout << "Hovering the button\r";
			states.texture = hover;
			if (newcontrol != currentcontrol && currentcontrol == CONTROL_STATES::CLICK) {
				insidetext.setOffset(sf::Vector2f(0, -2));
				insidetext.updatePosition();
				insidetext.update();
			}
		}
		break;
	case CONTROL_STATES::CLICK:
		if (newcontrol != currentcontrol) {
			//std::cout << "Pressing the button\r";
			states.texture = pressed;
			insidetext.updatePosition(sf::Vector2f(0, 2));
			insidetext.update();
			trigger();
		}
		break;
	}

	currentcontrol = newcontrol;
}
