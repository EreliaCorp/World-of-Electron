#pragma once

#include "jgl.h"
#include "widget/woe_debug_screen.h"

class Idle_activity : public jgl::Abstract_activity
{
private:
	enum class State
	{
		Resizing_manager,
		Activating_manager,
		Completed
	};
	jgl::String to_string(State p_state)
	{
		switch (p_state)
		{
		case (State::Resizing_manager):
			return ("Resizing manager");
		case (State::Activating_manager):
			return ("Activating manager");
		case (State::Completed):
			return ("Completed");
		default:
			return ("Undefined");
		}
	}
	State _state;

	void _set_state(State p_state)
	{
		Debug_screen::instance()->set_text("State : " + to_string(p_state), 0, 1);
		_state = p_state;
	}
public:
	Idle_activity();
	void execute();
	void on_transition();
};