#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class AnimatedSprite : public sf::Sprite {
	
	sf::Texture spriteSheet;
	sf::Sprite sprite;

	/*
	height of single sprite
	width of single sprite
	*/
	struct Anim {
		float speed;
		bool looping;
		std::vector<int> frames;
		sf::IntRect first_frame;
	};

	bool playing;
	bool flipped;
	int current_frame;
	int current_animation;
	std::vector<Anim> animations;

	float elapsedTime;

public:
	AnimatedSprite();
	~AnimatedSprite();

	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void animate(float dt);

	void setTexture(sf::Texture txt) { spriteSheet = txt; }
	sf::Texture getTexture() { return spriteSheet; }

	void setCurrentFrame(int c) { current_frame = c; }
	int getCurrentFrame() { return current_frame; }

	void setPlaying(bool p) { playing = p; }
	bool getPlaying() { return playing; }

	void setFlipped(bool f) { flipped = f; }
	bool getFlipped() { return flipped; }

	void addAnimation(std::vector<int> frames, float speed, sf::IntRect first_frame, bool looping = false, int index = -1);
};