#pragma once
#include <iostream>
#include "Framework/Room.h"
#include "Player.h"
#include "GameObjects/Octorock.h"

class Map_room : public Room {
public:
	Map_room(RoomManager* rm, sf::RenderWindow* window, InputHandler* input, std::string fold);
	~Map_room();

	void load(sf::Vector2f offset) override;
	void setPlayer(Player* pl);
	void removePlayer();
	void setPlayerPosition(sf::Vector2f pos);
	sf::Vector2f getPlayerPosition() { return sf::Vector2f(p->collider.rect.left, p->collider.rect.top); };
	sf::FloatRect getBound(int i) { return bounds[i]; }
	sf::Vector2f getCameraTopLeft() { return camera_top_left; }

	void sortGameObjects();
	void cullGameObjects();

	std::vector<GameObject*>* getGameObjects() { return &sceneObjects; }
	std::vector<Collision*>* getColliders() { return &sceneColliders; }

	void removeObject(GameObject* g);
	void freeMemory();

	void handleInput(float dt) override;
	void update(float dt) override;
	void draw() override;

	Tilemap* getTilemap() { return tilemap; }

	sf::Vector2f moveRoom(sf::Transform t);
	void setBounds(sf::Vector2f offset);
	sf::Vector2f getOffset() { return offset; }
	
private:
	sf::FloatRect bounds[4];

	sf::Vector2f camera_top_left;
	sf::Vector2f camera_bottom_right;

	// tilemap data
	Tilemap* tilemap = nullptr;
	JSONparser* tilemap_json = nullptr;
	sf::Vector2f offset;
	std::vector<int>* layers = nullptr;
	Tilemap::tilemap_data* tilemap_data = nullptr;
	sf::Shader* shader = nullptr;

	// player pointer
	Player* p = nullptr;
};
