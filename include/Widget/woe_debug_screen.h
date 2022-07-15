#pragma once

#include "jgl.h"

class Debug_screen : public jgl::Widget
{
private:
	static const jgl::Size_t C_NB_LINES = 26;
	static const jgl::Size_t C_NB_COLLUMNS = 5;

	jgl::Text_label* _lines[C_NB_LINES][C_NB_COLLUMNS];

	void _render();
	void _on_geometry_change();

	jgl::Bool _update();
	jgl::Bool _fixed_update();

public:
	Debug_screen(jgl::Widget* p_parent);

	void set_text(jgl::String p_text, jgl::Size_t p_collumns, jgl::Size_t p_line);
};