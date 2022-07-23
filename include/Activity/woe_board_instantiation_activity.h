#pragma once

#include "jgl.h"

#include "Utils/woe_configuration_file.h"
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
	State _state;
	const Configuration_file& _config_file;

public:
	Board_init_activity(const Configuration_file& p_config_file);
	void execute();
	void on_transition();
};
