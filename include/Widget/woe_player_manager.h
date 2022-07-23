#pragma once

#include "widget/woe_abstract_manager.h"
#include "widget/woe_iwidget.h"
#include "woe_network.h"

class Player_manager : public virtual Abstract_manager, public virtual IWidget
{
private:
	void _render();
	void _on_geometry_change();

	jgl::Bool _update();
	jgl::Bool _fixed_update();

	void _initiate_server();
	void _initiate_client();

	jgl::Bool _motion_command_send;
	void _push_motion_command(jgl::Vector2Int p_delta);
	void _pull_motion_command(Message& p_msg);

public:
	Player_manager(jgl::Widget* p_parent);
};