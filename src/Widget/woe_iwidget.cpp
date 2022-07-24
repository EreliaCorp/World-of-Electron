#include "Widget/woe_iwidget.h"
#include "structure/woe_engine.h"

jgl::Vector2Int IWidget::_convert_world_to_screen(jgl::Vector2 p_pos)
{
	jgl::Vector2Int result = p_pos * Node::SIZE + _area / 2;

	if (Engine::instance()->player() != nullptr)
	{
		result -= (Engine::instance()->player()->pos() - 0.5f) * Node::SIZE;
	}

	return (result);
}

jgl::Vector2Int IWidget::_convert_chunk_to_screen(jgl::Vector2Int p_pos)
{
	return (_convert_world_to_screen(p_pos * Chunk::C_SIZE));
}

jgl::Vector2 IWidget::_convert_screen_to_world(jgl::Vector2Int p_pos)
{
	jgl::Vector2 result = p_pos - _area / 2;
	result /= Node::SIZE;

	if (Engine::instance()->player() != nullptr)
	{
		result += (Engine::instance()->player()->pos() - 0.5f);
	}

	return (result);
}

jgl::Vector2Int IWidget::_convert_screen_to_chunk(jgl::Vector2Int p_pos)
{
	jgl::Vector2 result = _convert_screen_to_world(p_pos);
	result /= Chunk::C_SIZE;
	return (result.floor());
}

IWidget::IWidget(jgl::Widget* p_parent) : jgl::Widget(p_parent)
{

}