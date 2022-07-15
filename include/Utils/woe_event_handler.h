#pragma once

#include "jgl.h"

#include "woe_network.h"

#include "Utils/woe_configuration_file.h"

#include "structure/woe_engine.h"

enum class Event
{
	Initialize_network,
	Go_idle_mode
};

struct Context
{
	Configuration_file config;
	jgl::Bool downloading_map;
};

using Publisher = jgl::Singleton<jgl::Publisher<Event, Context>>;