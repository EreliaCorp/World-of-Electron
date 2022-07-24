#include "Widget/woe_main_application.h"
#include "woe_activities.h"
#include "Structure/woe_engine.h"
#include "Utils/woe_event_handler.h"
#include "widget/woe_debug_screen.h"
#include "woe_network.h"

void Main_application::_render()
{

}

void Main_application::_on_geometry_change()
{
	Debug_screen::instance()->set_geometry(0, _area);
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

void Main_application::_set_state(State p_state)
{
	Debug_screen::instance()->set_text("State : " + to_string(p_state), 0, 0);
	_state_machine.set_state(p_state);
}

Main_application::Main_application(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{
	_board_manager = new Board_manager(this);
	_entity_manager = new Entity_manager(this);
	_player_manager = new Player_manager(this);

	Debug_screen::instanciate(this);
	Debug_screen::instance()->activate();

	_state_machine.add_activity(State::Loading, new Loading_activity(_config_file));
	_state_machine.add_activity(State::Connection, new Connection_activity(this, _config_file));
	_state_machine.add_activity(State::Board_instantiation, new Board_init_activity(_config_file));
	_state_machine.add_activity(State::Player_instantiation, new Player_init_activity());
	_state_machine.add_activity(State::In_game, new Idle_activity());

	Event_handler::instanciate();

	Event_handler::subscribe(Event::Start_loading, [&]() {_set_state(State::Loading); });
	Event_handler::subscribe(Event::Start_connection, [&]() {_set_state(State::Connection); });
	Event_handler::subscribe(Event::Start_player_instantiation, [&]() {_set_state(State::Player_instantiation); });
	Event_handler::subscribe(Event::Start_board_instantiation, [&]() {_set_state(State::Board_instantiation); });
	Event_handler::subscribe(Event::Start_game, [&]() {_set_state(State::In_game); });

	Event_handler::notify(Event::Start_loading);
}