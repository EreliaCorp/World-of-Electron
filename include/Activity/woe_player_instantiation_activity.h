#pragma once

#include "jgl.h"

#include "Utils/woe_configuration_file.h"
#include "widget/woe_debug_screen.h"

class Player_init_activity : public jgl::Abstract_activity
{
private:
	enum class State
	{
		Request_data,
		Waiting_data,
		Completed
	};
	jgl::String to_string(State p_state)
	{
		switch (p_state)
		{
		case (State::Request_data):
			return ("Request data");
		case (State::Waiting_data):
			return ("Waiting data");
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
	Player_init_activity();
	void execute();
	void on_transition();
};