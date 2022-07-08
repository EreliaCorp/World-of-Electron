#include "widget/woe_main_application.h"
#include "woe_network.h"

void Main_application::Init_activity::execute()
{
	switch (_state)
	{
	case Internal_state::Waiting_client_connection:
	{
		if (Client_manager::instance()->client()->connection()->state() == Connection::State::Accepted)
		{
			_state = Internal_state::Asking_player_information;
		}
		else
			break;
	}
	case Internal_state::Asking_player_information:
	{
		if (_player_information_timer.started() == false || 
			_player_information_timer.timeout() == true)
		{
			g_context->id = -1;
			Message msg(Server_message::Request_player_information);

			Client_manager::send(msg);
			_player_information_timer.start();
		}
		break;
	}
	case Internal_state::Waiting_player_information:
	{
		if (g_context->id != -1)
		{

			_owner->set_state(Main_application::State::In_game);
		}
		break;
	}
	default:
	{

		break;
	}
	}
}

void Main_application::Init_activity::on_transition()
{
	_state = Internal_state::Waiting_client_connection;
}