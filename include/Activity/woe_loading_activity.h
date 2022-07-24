#pragma once

#include "jgl.h"

#include "Utils/woe_configuration_file.h"
#include "widget/woe_debug_screen.h"

class Loading_activity : public jgl::Abstract_activity
{
private:
	enum class State
	{
		Loading_config_file,
		Loading_chunk,
		Instantiate_engine,
		Completed
	};
	jgl::String to_string(State p_state)
	{
		switch (p_state)
		{
		case (State::Loading_config_file):
			return ("Loading configuration file");
		case (State::Loading_chunk):
			return ("Loading chunk");
		case (State::Instantiate_engine):
			return ("Instantiate engine");
		case (State::Completed):
			return ("Completed");
		default:
			return ("Undefined");
		}
	}
	State _state;
	Configuration_file& _config_file;

	void _set_state(State p_state)
	{
		Debug_screen::instance()->set_text("State : " + to_string(p_state), 0, 1);
		_state = p_state;
	}
public:
	Loading_activity(Configuration_file& p_config_file);
	void execute();
	void on_transition();
};