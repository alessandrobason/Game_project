#include "AnimatedSprite.h"

void AnimatedSprite::draw(sf::RenderWindow* w) {
	w->draw(sprite);
}

void AnimatedSprite::animate(float dt) {
	if (playing) {
		elapsedTime += dt;
		if (elapsedTime >= animations[current_animation].speed) {
			current_frame++;
			//std::cout << "size: " << animations[current_animation].frames.size() << "\n";
			if (current_frame >= (int)animations[current_animation].frames.size()) {
				if (animations[current_animation].looping) {
					current_frame = 0;
				}
				else {
					current_frame--;
					playing = false;
				}
			}
			//std::cout << "current frame: " << current_frame << "\n";
			//std::cout << "current animation: " << current_animation << "\n";
			//std::cout << animations[current_animation].frames[current_frame].left << " " << animations[current_animation].frames[current_frame].top << "\n";
			sprite.setTextureRect(animations[current_animation].frames[current_frame]);
			elapsedTime = 0;
		}
	}
}

void AnimatedSprite::setCurrentAnimation(int c) {
	if (current_animation != c) {
		current_animation = c;
		reset();
	}
}

void AnimatedSprite::addAnimation(std::vector<int> frames, float speed, bool looping, int index) {
	Anim a;
	a.speed = speed;
	a.looping = looping;
	for (size_t i = 0; i < frames.size(); i++) {
		int x = frames[i] % columns * width;
		int y = frames[i] / columns * height;
		a.frames.push_back(sf::IntRect(x, y, width, height));
	}
	std::cout << "\n";
	animations.push_back(a);
}

void AnimatedSprite::reset() {
	current_frame = 0;
	elapsedTime = 0;
	playing = true;
	sprite.setTextureRect(animations[current_animation].frames[current_frame]);
}