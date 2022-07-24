#include "Structure/woe_entity.h"
#include "structure/woe_engine.h"

Entity::Entity(Type p_type, jgl::Long p_id)
{
	_type = p_type;
	_id = p_id;
	_size = 0.8f;
	_direction = jgl::Vector2(1, 0);
	_move_speed = 100u;
	_is_moving = false;
}

void Entity::render(jgl::Vector2 p_anchor, jgl::Vector2 p_size, jgl::Float p_depth)
{
	switch (type())
	{
	case Entity::Type::Player:
	{
		jgl::draw_rectangle_color(jgl::Color::blue(), p_anchor - p_size * _size / 2, p_size * _size, p_depth);
		break;
	}
	case Entity::Type::Object:
	{
		jgl::draw_rectangle_color(jgl::Color(80, 80, 80), p_anchor - p_size * _size / 2, p_size * _size, p_depth);
		break;
	}
	case Entity::Type::Projectile:
	{
		jgl::draw_rectangle_color(jgl::Color::red(), p_anchor - p_size * _size / 2, p_size * _size, p_depth);
		break;
	}
	case Entity::Type::NPC:
	{
		jgl::draw_rectangle_color(jgl::Color(125, 125, 125), p_anchor - p_size * _size / 2, p_size * _size, p_depth);
		break;
	}
	}
}

void Entity::place(jgl::Vector2 p_pos)
{
	_pos = p_pos;
}

void Entity::update(jgl::Ulong p_ticks)
{
	if (_is_moving == true)
		update_pos(p_ticks);
	if (jgl::Application::active_application()->time() < _starting_time + _move_speed)
		_is_moving = false;
}

jgl::Bool Entity::can_move(jgl::Vector2 p_delta)
{
	for (jgl::Float x = -_size.x / 2.0f; x <= _size.x / 2.0f; x += _size.x)
	{
		for (jgl::Float y = -_size.y / 2.0f; y <= _size.y / 2.0f; y += _size.y)
		{
			if (Engine::instance()->board()->can_acces(_pos + p_delta + jgl::Vector2(x, y)) == false)
				return (false);
		}
	}
	return (true);
}

void Entity::update_pos(jgl::Ulong p_ticks)
{
	jgl::Vector2 delta_pos = _direction_unit * p_ticks;

	if (can_move(delta_pos) == true)
		_pos += delta_pos;
	else
		_is_moving = false;
}

void Entity::move(jgl::Vector2 p_delta)
{
	_direction = p_delta;
	_direction_unit = _direction / _move_speed;
	_starting_time = jgl::Application::active_application()->time();
	_is_moving = true;
}
