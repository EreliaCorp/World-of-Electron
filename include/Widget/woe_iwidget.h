#pragma once

#include "jgl.h"

class IWidget : public jgl::Widget
{
protected:
	jgl::Vector2Int _convert_world_to_screen(jgl::Vector2 p_pos);
	jgl::Vector2Int _convert_chunk_to_screen(jgl::Vector2Int p_pos);
	jgl::Vector2 _convert_screen_to_world(jgl::Vector2Int p_pos);
	jgl::Vector2Int _convert_screen_to_chunk(jgl::Vector2Int p_pos);

public:
	IWidget(jgl::Widget* p_parent);
};