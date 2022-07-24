#pragma once

#include "jgl.h"

#include "Utils/woe_configuration_file.h"
#include "widget/woe_debug_screen.h"

class Connection_activity : public jgl::Abstract_activity
{
private:
	enum class State
	{
		Starting_server,
		Starting_client,
		Waiting_for_connection,
		Completed
	};
	jgl::String to_string(State p_state)
	{
		switch (p_state)
		{
		case (State::Starting_server):
			return ("Starting server");
		case (State::Starting_client):
			return ("Starting client");
		case (State::Waiting_for_connection):
			return ("Waiting for connection");
		case (State::Completed):
			return ("Completed");
		default:
			return ("Undefined");
		}
	}
	State _state;
	const Configuration_file& _config_file;
	jgl::Widget* _manager_owner;

	void _set_state(State p_state)
	{
		Debug_screen::instance()->set_text("State : " + to_string(p_state), 0, 1);
		_state = p_state;
	}
public:
	Connection_activity(jgl::Widget* p_manager_owner, const Configuration_file& p_config_file);
	void execute();
	void on_transition();
};