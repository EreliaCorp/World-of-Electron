#pragma once

#include "jgl.h"

#include "Utils/woe_configuration_file.h"

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
	State _state;
	const Configuration_file& _config_file;
	jgl::Widget* _manager_owner;

public:
	Connection_activity(jgl::Widget* p_manager_owner, const Configuration_file& p_config_file);
	void execute();
	void on_transition();
};