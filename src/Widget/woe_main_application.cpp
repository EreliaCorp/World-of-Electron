#include "widget/woe_main_application.h"
#include "woe_network.h"

Main_application::Context* g_context = nullptr;

void Main_application::_render()
{

}

void Main_application::_on_geometry_change()
{

}

jgl::Bool Main_application::_update()
{
	State_machine::instance()->update();
	return (false);
}

jgl::Bool Main_application::_fixed_update()
{
	return (false);
}

void Main_application::_initialize_network()
{
	if (g_context->config.server_mode == true)
	{
		Server_manager::instanciate(this);
		Server_manager::instance()->start(g_context->config.server_port);
	}

	Client_manager::instanciate(this);
	Client_manager::instance()->start(g_context->config.server_address, g_context->config.server_port);
}

Main_application::Main_application(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	Publisher::instanciate();
	State_machine::instanciate();

	g_context = Publisher::instance()->context();
	g_context->config.decode("ressource/configuration/configuration");

	State_machine::instance()->add_activity(State::Init, new Init_activity(State_machine::instance()));

	State_machine::instance()->set_state(State::Init);

	_initialize_network();

}
