#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class AnimatedSprite {
protected:
	sf::Texture* spriteSheet = nullptr;
	sf::Sprite sprite;

	int rows = 0;
	int columns = 0;
	int width = 0;
	int height = 0;

	struct Anim {
		float speed;
		bool looping;
		std::vector<sf::IntRect> frames;
	};

	bool playing = true;
	bool flipped = false;
	int current_frame = 0;
	int current_animation = 0;
	std::vector<Anim> animations;

	float elapsedTime = 0;

public:
	AnimatedSprite() {}
	~AnimatedSprite() {}

	void draw(sf::RenderWindow* w);
	void animate(float dt);

	void setSpriteSheet(sf::Texture* txt, int c, int r) {
		spriteSheet = txt; 
		rows = r;
		columns = c; 
		width = spriteSheet->getSize().x / c;
		height = spriteSheet->getSize().y / r;
		sprite.setTexture(*spriteSheet);
		sprite.setTextureRect(sf::IntRect(0, 0, 0, 0));
	}

	sf::Texture getSpriteSheet() { return *spriteSheet; }
	sf::Sprite* getSprite() { return &sprite; }

	void setCurrentFrame(int c) { current_frame = c; }
	int getCurrentFrame() { return current_frame; }

	void setCurrentAnimation(int c);
	int getCurrentAnimation() { return current_animation; }

	void setPlaying(bool p) { playing = p; }
	bool getPlaying() { return playing; }

	void setFlipped(bool f) { flipped = f; }
	bool getFlipped() { return flipped; }

	void addAnimation(std::vector<int> frames, float speed, bool looping = true, int index = -1);

	void reset();
};