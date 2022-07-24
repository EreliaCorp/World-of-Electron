#include "activity/woe_connection_activity.h"
#include "widget/woe_main_application.h"
#include "woe_network.h"

Connection_activity::Connection_activity(jgl::Widget* p_manager_owner, const Configuration_file& p_config_file) : jgl::Abstract_activity(),
_state(State::Starting_server),
_manager_owner(p_manager_owner),
_config_file(p_config_file)
{

}

void Connection_activity::execute()
{
	switch (_state)
	{
	case State::Starting_server:
	{
		if (_config_file.server_mode == true)
		{
			THROW_INFORMATION("Starting the server ...");
			Server_manager::instance()->start(_config_file.server_port);
			Server_manager::instance()->activate();
			THROW_INFORMATION("Server succesfully started");
			Event_handler::notify(Event::Configure_server_interaction);
		}
		_set_state(State::Starting_client);
		break;
	}
	case State::Starting_client:
	{
		THROW_INFORMATION("Starting the client ...");
		Client_manager::instance()->start(_config_file.server_address, _config_file.server_port);
		Client_manager::instance()->activate();
		THROW_INFORMATION("Client succesfully started");
		Event_handler::notify(Event::Configure_client_interaction);
		_set_state(State::Waiting_for_connection);
		break;
	}
	case State::Waiting_for_connection:
	{
		THROW_INFORMATION("Waiting for response ...");
		if (Client_manager::instance()->client()->connection()->state() == Connection::State::Accepted)
		{
			THROW_INFORMATION("Connection complete");
			_set_state(State::Completed);
		}
		break;
	}

	case State::Completed:
	{
		THROW_INFORMATION("=== CONNECTION COMPLETED ===");
		THROW_INFORMATION("");
		Event_handler::notify(Event::Start_board_instantiation);
		break;
	}
	}
}
void Connection_activity::on_transition()
{
	THROW_INFORMATION("=== ENTERING CONNECTION ACTIVITY ===");
	if (_config_file.server_mode == true)
	{
		THROW_INFORMATION("Instantiation of the server manager");
		Server_manager::instanciate(_manager_owner);
	}
	THROW_INFORMATION("Instantiation of the client manager");
	Client_manager::instanciate(_manager_owner);
	_set_state(State::Starting_server);
}