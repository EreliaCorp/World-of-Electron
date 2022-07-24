#include "activity/woe_player_instantiation_activity.h"
#include "structure/woe_engine.h"
#include "Utils/woe_event_handler.h"
#include "woe_network.h"

Player_init_activity::Player_init_activity() : jgl::Abstract_activity(),
	_state(State::Request_data)
{

}

void Player_init_activity::execute()
{
	switch (_state)
	{
	case State::Request_data:
	{
		THROW_INFORMATION("Requesting player data");
		Client_manager::send(Server_message::Player_id);
		_set_state(State::Waiting_data);
		break;
	}
	case State::Waiting_data:
	{
		THROW_INFORMATION("Waiting for player information ...");
		if (Engine::instance()->player_id() != -1)
		{
			THROW_INFORMATION("Player information received");
			THROW_INFORMATION("Player ID : " + jgl::itoa(Engine::instance()->player_id()));
			_set_state(State::Completed);
		}
		break;
	}

	case State::Completed:
	{
		THROW_INFORMATION("=== PLAYER INITIATION COMPLETED ===");
		THROW_INFORMATION("");
		Event_handler::notify(Event::Start_game);
		break;
	}
	}
}

void Player_init_activity::on_transition()
{
	THROW_INFORMATION("=== ENTERING PLAYER INITIATION ACTIVITY ===");
	_set_state(State::Request_data);
}