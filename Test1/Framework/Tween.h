#pragma once

class Tween {
public:
	static float lerp(float v0, float v1, float t) {
		return v0 * (1 - t) + v1 * t;
	}
};