#include "activity/woe_idle_activity.h"
#include "Utils/woe_event_handler.h"

Idle_activity::Idle_activity() : jgl::Abstract_activity(),
	_state(State::Resizing_manager)
{

}

void Idle_activity::execute()
{
	switch (_state)
	{
	case State::Resizing_manager:
	{
		THROW_INFORMATION("Resizing manager");
		Event_handler::notify(Event::Resize_manager_widget);
		_set_state(State::Activating_manager);
		break;
	}
	case State::Activating_manager:
	{
		THROW_INFORMATION("Activating manager");
		Event_handler::notify(Event::Activating_manager_widget);
		_set_state(State::Completed);
		break;
	}

	case State::Completed:
	{
		break;
	}
	}
}
void Idle_activity::on_transition()
{
	THROW_INFORMATION("=== ENTERING IDLE ACTIVITY ===");
	_set_state(State::Resizing_manager);
}