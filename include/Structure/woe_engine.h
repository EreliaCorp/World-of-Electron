#pragma once

#include "jgl.h"

#include "woe_network.h"
#include "structure/woe_tilemap.h"

class Engine : public jgl::Singleton<Engine>
{
	friend class jgl::Singleton<Engine>;

private:
	Tilemap* _map;

	Engine();

public:
	Tilemap* map() { return (_map); }
};