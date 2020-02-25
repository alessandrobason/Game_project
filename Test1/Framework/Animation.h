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
	std::string current_animation = "";
	std::map<std::string, Anim> animations;

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

	void setCurrentAnimation(std::string name);
	std::string getCurrentAnimation() { return current_animation; }

	void setPlaying(bool p) { playing = p; }
	bool getPlaying() { return playing; }

	void setFlipped(bool f) { flipped = f; }
	bool getFlipped() { return flipped; }

	void addAnimation(std::string name, std::vector<int> frames, float speed, bool looping = true);

	void reset();
};

class AnimatedTile : public sf::Drawable, public sf::Transformable {
public:
	AnimatedTile() {
		tiles.setPrimitiveType(sf::Quads);
	}
	~AnimatedTile() {}

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	void appendTile(sf::Vertex t[4], std::vector<sf::IntRect> f, int s) {
		for(size_t i=0; i<4; i++)
			tiles.append(t[i]);
		frames.push_back(f);
		speeds.push_back((float)s / 1000);
		elapsedTimes.push_back(0);
		current_frames.push_back(0);
	}

	void animate(float dt);
	void reset();

	void setTilemap(sf::Texture* t) { m_tilemap = t; }
	void setPlaying(bool p) { playing = p; }
	bool getPlaying() { return playing; }

private:
	sf::VertexArray tiles;
	sf::Texture* m_tilemap = nullptr;
	std::vector<std::vector<sf::IntRect>> frames;
	std::vector<float> speeds;
	std::vector<float> elapsedTimes;
	std::vector<int> current_frames;
	bool playing = true;

};