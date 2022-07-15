#pragma once

#include "jgl.h"
#include "Widget/woe_manager_widget.h"

class Tilemap_manager : public Manager
{
private:
	void _render();
	void _on_geometry_change();

	jgl::Bool _update();
	jgl::Bool _fixed_update();
	
	jgl::Sprite_sheet* tmp;

	void _initialize_server();
	void _initialize_client();

	void _load_map_data();

public:
	Tilemap_manager(jgl::Widget* p_parent);
};