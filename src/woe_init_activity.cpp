#include "Activity/woe_init_activity.h"
#include "Utils/woe_event_handler.h"
#include "Structure/woe_engine.h"

void Init_activity::execute()
{
	switch (_state)
	{
	case State::Init:
	{
		_set_state(State::Loading_configuration_files);
		break;
	}
	case State::Loading_configuration_files:
	{
		Publisher::instance()->context()->config.decode("ressource/configuration/config.conf");

		Client_manager::instanciate(nullptr);
		Client_manager::instance()->activate();

		if (Publisher::instance()->context()->config.server_mode == true)
		{
			Server_manager::instanciate(nullptr);
			Server_manager::instance()->activate();

			_set_state(State::Init_networking_structure);
		}
		else
			_set_state(State::Connection_to_server);
		break;
	}
	case State::Init_networking_structure:
	{
		if (Publisher::instance()->context()->config.server_mode == true)
		{
			Server_manager::instance()->start(
				Publisher::instance()->context()->config.server_port
			);
		}

		_set_state(State::Connection_to_server);
		break;
	}
	case State::Connection_to_server:
	{
		Client_manager::instance()->start(
			Publisher::instance()->context()->config.server_address,
			Publisher::instance()->context()->config.server_port);
		_set_state(State::Waiting_connection_to_server);
		break;
	}
	case State::Waiting_connection_to_server:
	{
		if (Client_manager::client()->connection()->state() == Connection::State::Accepted)
			_set_state(State::Connection_complete);
		break;
	}
	case State::Connection_complete:
	{
		Publisher::instance()->notify(Event::Initialize_network);

		_set_state(State::Loading_engine);
		break;
	}
	case State::Loading_engine:
	{
		Engine::instanciate();

		if (Publisher::instance()->context()->config.server_mode == true)
			_set_state(State::Loading_map);
		else
			_set_state(State::Downloading_map);

		break;
	}
	case State::Loading_map:
	{
		Engine::instance()->map()->generate();
		_set_state(State::Init_complete);
		break;
	}
	case State::Downloading_map:
	{
		Client_manager::client()->send(Server_message::Request_map_content);
		Publisher::instance()->context()->downloading_map = true;
		_set_state(State::Waiting_map_download);
		break;
	}
	case State::Waiting_map_download:
	{
		if (Publisher::instance()->context()->downloading_map == false)
			_set_state(State::Init_complete);
		break;
	}
	case State::Init_complete:
	{
		Publisher::instance()->notify(Event::Go_idle_mode);
		break;
	}
	}
}