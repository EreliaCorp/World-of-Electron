#pragma once

#include "jgl.h"
#include "widget/woe_debug_screen.h"

class Init_activity : public jgl::Abstract_activity
{
private:
	enum State
	{
		Init,
		Loading_configuration_files,
		Init_networking_structure,
		Connection_to_server,
		Waiting_connection_to_server,
		Connection_complete,
		Loading_engine,
		Loading_map,
		Downloading_map,
		Waiting_map_download,
		Init_complete
	};
	jgl::String to_string(const Init_activity::State& p_value)
	{
		switch (p_value)
		{
		case State::Init:
			return ("Init");
		case State::Loading_configuration_files:
			return ("Loading configuration files");
		case State::Init_networking_structure:
			return ("Initializing network structure");
		case State::Connection_to_server:
			return ("Init connection");
		case State::Waiting_connection_to_server:
			return ("Connecting...");
		case State::Connection_complete:
			return ("Connection completed");
		case State::Loading_engine:
			return ("Loading engine");
		case State::Loading_map:
			return ("Loading map");
		case State::Downloading_map:
			return ("Init map download");
		case State::Waiting_map_download:
			return ("Downloading the map...");
		case State::Init_complete:
			return ("Init complete");
		default:
			return ("Unknow state");
		}
	}

	State _state;
	Debug_screen* _debug_screen;

	void _set_state(State p_state)
	{
		_state = p_state;
		_debug_screen->set_text("Activity state : " + to_string(p_state), 1, 0);
	}

public:
	Init_activity(Debug_screen* p_debug_screen)
	{
		_debug_screen = p_debug_screen;
	}

	void execute();

	void on_transition()
	{
		_set_state(State::Init);
	}
};