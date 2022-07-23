#pragma once

#include "widget/woe_abstract_manager.h"
#include "widget/woe_iwidget.h"

class Board_manager : public virtual Abstract_manager, public virtual IWidget
{
private:
	void _render();
	void _on_geometry_change();

	jgl::Bool _update();
	jgl::Bool _fixed_update();

	void _initiate_server();
	void _initiate_client();

public:
	Board_manager(jgl::Widget* p_parent);
};