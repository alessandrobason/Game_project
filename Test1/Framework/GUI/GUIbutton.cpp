#include "GUIbutton.h"

GUIbutton::GUIbutton(const GUIbutton& copy) : GUItiledelement(copy) {
	id = copy.id;
	tilesize = copy.tilesize;
	normal = copy.normal;
	hover = copy.hover;
	pressed = copy.pressed;
	insidetext = copy.insidetext;
	callback = copy.callback;
	callbackobject = copy.callbackobject;
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
	callback(*callbackobject, id);
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
			if (newcontrol != currentcontrol && currentcontrol == CONTROL_STATES::CLICKDOWN) {
				insidetext.updatePosition(sf::Vector2f(0, -2));
				insidetext.update();
			}
		}
		break;
	case CONTROL_STATES::HOVER:
		if (newcontrol != currentcontrol) {
			//std::cout << "Hovering the button\r";
			states.texture = hover;
			if (newcontrol != currentcontrol && currentcontrol == CONTROL_STATES::CLICKDOWN) {
				insidetext.updatePosition(sf::Vector2f(0, -2));
				insidetext.update();
			}
		}
		break;
	case CONTROL_STATES::CLICKDOWN:
		if (newcontrol != currentcontrol) {
			states.texture = pressed;
			insidetext.updatePosition(sf::Vector2f(0, 2));
			insidetext.update();
		}
		break;
	case CONTROL_STATES::CLICKUP:
		if (newcontrol != currentcontrol) {
			trigger();
		}
		break;
	}

	currentcontrol = newcontrol;
}
