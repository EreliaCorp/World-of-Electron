#pragma once

#include "jgl.h"

#include "Utils/woe_event_handler.h"
#include "widget/woe_iwidget.h"

class Manager : public IWidget
{
private:
	virtual void _initialize_server() = 0;
	virtual void _initialize_client() = 0;

public:
	Manager(jgl::Widget* p_parent) : IWidget(p_parent)
	{
		Publisher::instance()->subscribe(Event::Go_idle_mode, [&](Context& p_context) {
				activate();
			});

		Publisher::instance()->subscribe(Event::Initialize_network, [&](Context& p_context) {
				initialize_network();
			});
	}
	void initialize_network()
	{
		if (Publisher::instance()->context()->config.server_mode == true)
			_initialize_server();
		_initialize_client();
	}
};