#include "Structure/woe_entity.h"

Entity::Entity(Type p_type, jgl::Long p_id)
{
	_type = p_type;
	_id = p_id;
	_destination = 0;
	_move_speed = 100u;
	_is_moving = false;
	_join_destination();
}

void Entity::render(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size, jgl::Float p_depth)
{
	switch (type())
	{
	case Entity::Type::Red:
	{
		jgl::draw_rectangle_color(jgl::Color::red(), p_anchor, p_size, p_depth);
		break;
	}
	case Entity::Type::Green:
	{
		jgl::draw_rectangle_color(jgl::Color::green(), p_anchor, p_size, p_depth);
		break;
	}
	case Entity::Type::Blue:
	{
		jgl::draw_rectangle_color(jgl::Color::blue(), p_anchor, p_size, p_depth);
		break;
	}
	case Entity::Type::Grey:
	{
		jgl::draw_rectangle_color(jgl::Color(125, 125, 125), p_anchor, p_size, p_depth);
		break;
	}
	}
}

void Entity::place(jgl::Vector2 p_pos)
{
	_pos = p_pos;
	_starting_pos = _pos;
	_destination = _pos;
}

void Entity::_join_destination()
{
	_pos = _destination.round();
}

void Entity::update()
{

}

void Entity::update_pos()
{
	jgl::Ulong time = jgl::Application::active_application()->time();

	_pos = jgl::lerp(_starting_pos, _destination, time - _start_motion_time, _move_speed);

	if (_start_motion_time + _move_speed <= jgl::Application::active_application()->time())
	{
		_join_destination();
		_is_moving = false;
	}
}

void Entity::move(jgl::Vector2 p_delta)
{
	_starting_pos = _pos;
	_movement = p_delta;
	_destination = _pos + p_delta;
	_start_motion_time = jgl::Application::active_application()->time();
	_is_moving = true;
}
