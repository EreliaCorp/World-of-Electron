#pragma once

#include "jgl.h"

#include "Utils/woe_configuration_file.h"
#include "widget/woe_debug_screen.h"
#include "Structure/woe_board.h"

class Board_init_activity : public jgl::Abstract_activity
{
private:
	enum class State
	{
		Reset_data,
		Generate_data,
		Request_data,
		Waiting_data,
		Unbake,
		Completed
	};
	jgl::String to_string(State p_state)
	{
		switch (p_state)
		{
		case (State::Reset_data):
			return ("Reset data");
		case (State::Generate_data):
			return ("Generate data");
		case (State::Request_data):
			return ("Request data");
		case (State::Waiting_data):
			return ("Waiting data");
		case (State::Unbake):
			return ("Unbake");
		case (State::Completed):
			return ("Completed");
		default:
			return ("Undefined");
		}
	}
	State _state;
	const Configuration_file& _config_file;

	void _set_state(State p_state)
	{
		Debug_screen::instance()->set_text("State : " + to_string(p_state), 0, 1);
		_state = p_state;
	}

public:
	Board_init_activity(const Configuration_file& p_config_file);
	void execute();
	void on_transition();
};
