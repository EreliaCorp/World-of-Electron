#pragma once

#include "jgl.h"

class Idle_activity : public jgl::Abstract_activity
{
private:
	enum class State
	{
		Resizing_manager,
		Activating_manager,
		Completed
	};
	State _state;

public:
	Idle_activity();
	void execute();
	void on_transition();
};