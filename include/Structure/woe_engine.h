#pragma once

#include "jgl.h"

#include "structure/woe_tilemap.h"

class Engine
{
private:
	Tilemap* _map;

public:
	Engine();
	Tilemap* map() { return (_map); }
};