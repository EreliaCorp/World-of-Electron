#pragma once

#include "jgl.h"

#include "Utils/woe_configuration_file.h"

class Player_init_activity : public jgl::Abstract_activity
{
private:
	enum class State
	{
		Request_data,
		Waiting_data,
		Completed
	};
	State _state;

public:
	Player_init_activity();
	void execute();
	void on_transition();
};