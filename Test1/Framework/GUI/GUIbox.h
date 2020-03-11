#pragma once
#include "GUItiledelement.h"

class GUIbox : public GUItiledelement {
public:
	GUIbox() : GUItiledelement() {}
	GUIbox(const GUIbox& copy);

	void setTileSize(int t) { tilesize = t; }

	void load();
};