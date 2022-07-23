#include "Widget/woe_main_application.h"
#include "woe_activities.h"
#include "Structure/woe_engine.h"
#include "Utils/woe_event_handler.h"
#include "woe_network.h"

void Main_application::_render()
{

}

void Main_application::_on_geometry_change()
{

}

jgl::Bool Main_application::_update()
{
	_state_machine.update();

	return (false);
}

jgl::Bool Main_application::_fixed_update()
{
	return (false);
}

void Main_application::_initiate_server()
{
}

void Main_application::_initiate_client()
{
}

Main_application::Main_application(jgl::Widget* p_parent) : jgl::Widget(p_parent), Abstract_manager(p_parent)
{
	_state_machine.add_activity(State::Loading, new Loading_activity(_config_file));
	_state_machine.add_activity(State::Connection, new Connection_activity(this, _config_file));
	_state_machine.add_activity(State::Player_instantiation, new Player_init_activity());
	_state_machine.add_activity(State::Board_instantiation, new Board_init_activity(_config_file));
	_state_machine.add_activity(State::Idle, new Idle_activity());

	Event_handler::instanciate();

	Event_handler::subscribe(Event::Start_loading, [&]() {_state_machine.set_state(State::Loading); });
	Event_handler::subscribe(Event::Start_connection, [&]() {_state_machine.set_state(State::Connection); });
	Event_handler::subscribe(Event::Start_player_instantiation, [&]() {_state_machine.set_state(State::Player_instantiation); });
	Event_handler::subscribe(Event::Start_board_instantiation, [&]() {_state_machine.set_state(State::Board_instantiation); });
	Event_handler::subscribe(Event::Start_idle, [&]() {_state_machine.set_state(State::Idle); });

	Event_handler::notify(Event::Start_loading);

	_board_manager = new Board_manager(this);
	_entity_manager = new Entity_manager(this);
	_player_manager = new Player_manager(this);
}