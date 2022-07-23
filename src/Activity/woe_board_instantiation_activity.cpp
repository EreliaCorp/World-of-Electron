#include "activity/woe_board_instantiation_activity.h"
#include "widget/woe_main_application.h"
#include "structure/woe_engine.h"
#include "woe_network.h"

Board_init_activity::Board_init_activity(const Configuration_file& p_config_file) : jgl::Abstract_activity(),
	_state(State::Reset_data),
	_config_file(p_config_file)
{

}

void Board_init_activity::execute()
{
	switch (_state)
	{
	case State::Reset_data:
	{
		THROW_INFORMATION("Reseting board information");
		if (_config_file.server_mode == true)
			_state = State::Generate_data;
		else
			_state = State::Request_data;
		break;
	}
	case State::Generate_data:
	{
		THROW_INFORMATION("Generating board data");
		Engine::instance()->create_board();
		Engine::instance()->board()->generate();
		_state = State::Unbake;
		break;
	}
	case State::Request_data:
	{
		THROW_INFORMATION("Requesting board data");
		Client_manager::send(Server_message::Board_data);
		_state = State::Waiting_data;
		break;
	}
	case State::Waiting_data:
	{
		THROW_INFORMATION("Waiting for board information ...");
		if (Engine::instance()->board() != nullptr)
		{
			THROW_INFORMATION("Board information received");
			_state = State::Unbake;
		}
		break;
	}
	case State::Unbake:
	{
		THROW_INFORMATION("Unbaking board");
		Engine::instance()->board()->unbake();
		_state = State::Completed;
		break;
	}

	case State::Completed:
	{
		THROW_INFORMATION("=== BOARD INITIATION COMPLETED ===");
		THROW_INFORMATION("");
		Event_handler::notify(Event::Start_idle);
		break;
	}
	}
}

void Board_init_activity::on_transition()
{
	THROW_INFORMATION("=== ENTERING BOARD INITIATION ACTIVITY ===");
	_state = State::Reset_data;
}