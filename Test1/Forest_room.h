#pragma once
#include <iostream>
#include "Framework/Room.h"
#include "Player.h"
#include "GameObjects/Tree.h"

class Forest_room : public Room {
public:
	Forest_room(RoomManager* rm, sf::RenderWindow* window, InputHandler* input, std::string fold);
	~Forest_room() {
		//p = nullptr;
	}

	void load(sf::Vector2f offset) override;
	void setPlayer(Player* pl);
	void removePlayer();
	void setPlayerPosition(sf::Vector2f pos);
	sf::Vector2f getPlayerPosition() { return p->getSprite()->getPosition(); };
	sf::FloatRect getBound(int i) { return bounds[i]; }

	void sortGameObjects();
	void cullGameObjects();

	void handleInput(float dt) override;
	void update(float dt) override;
	void draw() override;
	
private:
	sf::FloatRect bounds[4];

	Player* p;
};
