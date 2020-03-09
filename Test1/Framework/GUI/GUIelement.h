#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <assert.h>

class GUIelement {
public:
	enum class CONTROL_STATES {
		NONE,
		OVER,
		CLICK
	};

	bool enabled = true;

	virtual void trigger() {}

protected:
};