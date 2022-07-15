#pragma once

#include "jgl.h"

#include "Widget/woe_tilemap_manager.h"
#include "Widget/woe_debug_screen.h"

class Main_application : public jgl::Widget
{
private:
	enum class State
	{
		Init,
		Idle
	};

	jgl::String to_string(const State& p_value)
	{
		switch (p_value)
		{
		case State::Init:
			return ("Init");
		case State::Idle:
			return ("Idle");
		default:
			return ("Unknow state");
		}
	}

	using State_machine = jgl::State_machine<State>;

	jgl::Timer _update_fps_timer;
	jgl::Int _render_cmpt;
	jgl::Int _update_cmpt;

	State_machine* _state_machine;

	Debug_screen* _debug_screen;
	Tilemap_manager* _tilemap_manager;

	void _render();
	void _on_geometry_change();

	jgl::Bool _update();
	jgl::Bool _fixed_update();


public:
	Main_application(jgl::Widget* p_parent);

	void set_state(State p_state);
};