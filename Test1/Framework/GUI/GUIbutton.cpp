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
	insidetext.load(text, 8);
	insidetext.update();
}

void GUIbutton::draw(sf::RenderWindow* w) {
	GUIelement::draw(w);
	insidetext.draw(w);
}

void GUIbutton::trigger() {
	//std::cout << "ciao sono un bottone\n";
}

void GUIbutton::load() {
	GUItiledelement::load();
	if (states.texture == NULL) {
		std::cout << "Texture not loaded\n";
		abort();
	}

	nineslice(tilesize);
	alignElement();
}

void GUIbutton::changeControlState(CONTROL_STATES newcontrol) {
	switch (newcontrol)	{
	case CONTROL_STATES::NONE:
		if (newcontrol != currentcontrol) {
			//std::cout << "Nothing the button\r";
			states.texture = normal;
		}
		break;
	case CONTROL_STATES::HOVER:
		if (newcontrol != currentcontrol) {
			//std::cout << "Hovering the button\r";
			states.texture = hover;
		}
		break;
	case CONTROL_STATES::CLICK:
		if (newcontrol != currentcontrol) {
			//std::cout << "Pressing the button\r";
			states.texture = pressed;
			trigger();
		}
		break;
	}

	currentcontrol = newcontrol;
}
