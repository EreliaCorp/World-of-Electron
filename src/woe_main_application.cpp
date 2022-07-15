#include "Widget/woe_main_application.h"
#include "woe_network.h"
#include "Activity/woe_init_activity.h"
#include "Utils/woe_event_handler.h"
#include "Structure/woe_tilemap.h"

void Main_application::_render()
{
	_render_cmpt++;
}

void Main_application::_on_geometry_change()
{
	if (Engine::instance() != nullptr)
		Engine::instance()->map()->unbake();
	_debug_screen->set_geometry(0, _area);
	_tilemap_manager->set_geometry(0, _area);
}

jgl::Bool Main_application::_update()
{
	_state_machine->update();

	if (jgl::Application::active_application()->keyboard().get_key(jgl::Key::Key_1) == jgl::Input_status::Release &&
		Node::SIZE > 2)
	{
		Node::SIZE /= 2;
		_debug_screen->set_text("Node size : " + jgl::itoa(Node::SIZE), 0, 1);
		Engine::instance()->map()->unbake();
	}
	if (jgl::Application::active_application()->keyboard().get_key(jgl::Key::Key_3) == jgl::Input_status::Release &&
		Node::SIZE < 64)
	{
		Node::SIZE *= 2;
		_debug_screen->set_text("Node size : " + jgl::itoa(Node::SIZE), 0, 1);
		Engine::instance()->map()->unbake();
	}
	_update_cmpt++;

	return (false);
}

jgl::Bool Main_application::_fixed_update()
{
	if (_update_fps_timer.timeout() == true || _update_fps_timer.started() == false)
	{
		_debug_screen->set_text("Render FPS : " + jgl::itoa(_render_cmpt), 0, 2);
		_debug_screen->set_text("Update FPS : " + jgl::itoa(_update_cmpt), 0, 3);
		_render_cmpt = 0;
		_update_cmpt = 0;
		_update_fps_timer.start();
	}

	return (false);
}

void Main_application::set_state(State p_state)
{
	_state_machine->set_state(p_state);
	_debug_screen->set_text("State : " + to_string(p_state), 0, 0);
	_debug_screen->set_text("", 1, 0);
}

Main_application::Main_application(jgl::Widget* p_parent) : jgl::Widget(p_parent),
	_update_fps_timer(1000)
{
	Publisher::instanciate();

	_debug_screen = new Debug_screen(this);
	_debug_screen->activate();

	Publisher::instance()->subscribe(Event::Go_idle_mode, [&](Context& p_context) {
			set_state(State::Idle);
		});

	_state_machine = new State_machine(State::Init);
	_state_machine->add_activity(State::Init, new Init_activity(_debug_screen));

	_tilemap_manager = new Tilemap_manager(this);
}