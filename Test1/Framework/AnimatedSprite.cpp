#include "AnimatedSprite.h"

AnimatedSprite::AnimatedSprite() {

}

AnimatedSprite::~AnimatedSprite() {}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();

	states.texture = &spriteSheet;
	target.draw(sprite);
}

void AnimatedSprite::animate(float dt) {
	if (playing) {
		elapsedTime += dt;
		if (elapsedTime >= animations[current_animation].speed) {
			current_frame++;
			if (current_frame >= (int)animations[current_animation].frames.size()) {
				if (animations[current_animation].looping) {
					current_frame = 0;
				}
				else {
					current_frame--;
					playing = false;
				}
			}
			// TODO
			sprite.setTextureRect(animations[current_animation].first_frame);
			elapsedTime = 0;
		}
	}
}

void AnimatedSprite::addAnimation(std::vector<int> frames, float speed, sf::IntRect first_frame, bool looping, int index) {
	Anim a;
	a.speed = speed;
	a.looping = looping;
	a.frames = frames;
	a.first_frame = first_frame;
	animations.push_back(a);
	//for (size_t i = 0; i < frames.size(); i++) {
	//}
}