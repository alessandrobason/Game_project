#pragma once

template<class T>
class Tweening {
	T initalvalue;
	T finalvalue;
	float remainingtime = 0.f;
	float totaltime = 0.f;
	float percentagepassed = 0.f;
	bool finished = false;

	T current;

public:
	Tweening() {}
	~Tweening() {}

	Tweening(T i, T f, float t) {
		initalvalue = i;
		finalvalue = f;
		remainingtime = t;
		totaltime = t;
	}

	void update(float dt) {
		remainingtime -= dt;
		percentagepassed = 1 - (remainingtime / totaltime);
		current = interpolate();
		if (percentagepassed > 1.f) finished = true;
	}

	void setTotalTime(float t) {
		totaltime = t;
	}

	float getPercentagePassed() { return percentagepassed; }
	T getValue() { return current; }
	bool isfinished() { return finished; }

	T interpolate() {
		return initalvalue * (1 - percentagepassed) + finalvalue * percentagepassed;
	}
};