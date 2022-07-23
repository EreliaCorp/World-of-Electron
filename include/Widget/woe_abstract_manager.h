#pragma once

#include "jgl.h"

#include "Utils/woe_event_handler.h"

class Abstract_manager : public virtual jgl::Widget
{
private:

	virtual void _initiate_server() = 0;
	virtual void _initiate_client() = 0;

public:
	Abstract_manager(jgl::Widget* p_parent) : jgl::Widget(p_parent)
	{
		Event_handler::instanciate();

		Event_handler::subscribe(Event::Configure_server_interaction, [&]() { _initiate_server(); });
		Event_handler::subscribe(Event::Configure_client_interaction, [&]() { _initiate_client(); });
		Event_handler::subscribe(Event::Resize_manager_widget, [&]() { set_geometry(0, jgl::Application::active_application()->size()); });
		Event_handler::subscribe(Event::Activating_manager_widget, [&]() { activate(); });
		Event_handler::subscribe(Event::Desactivating_manager_widget, [&]() { desactivate(); });
	}
};