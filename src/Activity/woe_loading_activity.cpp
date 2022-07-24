#include "activity/woe_loading_activity.h"
#include "widget/woe_main_application.h"
#include "structure/woe_engine.h"

Loading_activity::Loading_activity(Configuration_file& p_config_file) : jgl::Abstract_activity(),
_state(State::Loading_config_file),
_config_file(p_config_file)
{

}

void Loading_activity::execute()
{
	switch (_state)
	{
	case State::Loading_config_file:
	{
		THROW_INFORMATION("Loading configuration file");
		_config_file.decode("ressource/configuration/config.conf");
		THROW_INFORMATION("Conf readed :");
		THROW_INFORMATION("Server address : " + _config_file.server_address);
		THROW_INFORMATION("Server port : " + jgl::itoa(_config_file.server_port));
		THROW_INFORMATION("Server mode : " + jgl::String(_config_file.server_mode == true ? "true" : "false"));
		_set_state(State::Loading_chunk);
		break;
	}
	case State::Loading_chunk:
	{
		THROW_INFORMATION("Loading texture files");
		Node::SIZE = 48;
		Chunk::add_node(new Node(0, jgl::Vector2Int(0, 0), true, Node::OBSTACLE, 1));
		_set_state(State::Instantiate_engine);
		break;
	}
	case State::Instantiate_engine:
	{
		THROW_INFORMATION("Instanciate engine");
		Engine::instanciate();
		_set_state(State::Completed);
		break;
	}
	case State::Completed:
	{
		THROW_INFORMATION("=== LOADING COMPLETED ===");
		THROW_INFORMATION("");
		Event_handler::notify(Event::Start_connection);
		break;
	}
	}
}

void Loading_activity::on_transition()
{
	THROW_INFORMATION("=== ENTERING LOADING ACTIVITY ===");
	_set_state(State::Loading_config_file);
}