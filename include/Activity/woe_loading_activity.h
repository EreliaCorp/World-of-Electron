#pragma once

#include "jgl.h"

#include "Utils/woe_configuration_file.h"

class Loading_activity : public jgl::Abstract_activity
{
private:
	enum class State
	{
		Loading_config_file,
		Loading_texture,
		Loading_chunk,
		Instantiate_engine,
		Completed
	};
	State _state;
	Configuration_file& _config_file;

public:
	Loading_activity(Configuration_file& p_config_file);
	void execute();
	void on_transition();
};