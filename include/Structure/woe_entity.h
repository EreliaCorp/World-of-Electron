#pragma once

#include "jgl.h"

class Entity
{
public:
	enum class Type
	{
		Object,
		NPC,
		Player,
		Projectile,
	};
protected:
	Type _type;
	jgl::Long _id;
	jgl::Bool _is_moving;
	jgl::Vector2 _pos;
	jgl::Vector2 _size;

	//Movement relative attribut
	jgl::Vector2 _direction;
	jgl::Vector2 _direction_unit;
	jgl::Ulong _move_speed;
	jgl::Ulong _starting_time;

public:
	Entity(Type p_type, jgl::Long p_id);

	virtual void update(jgl::Ulong p_ticks);
	void update_pos(jgl::Ulong p_ticks);
	jgl::Bool can_move(jgl::Vector2 p_delta);

	virtual void place(jgl::Vector2 p_pos);
	void move(jgl::Vector2 p_delta);

	virtual void render(jgl::Vector2 p_anchor, jgl::Vector2 p_size, jgl::Float p_depth);

	void set_id(jgl::Long p_id) { _id = p_id; }
	void set_size(jgl::Vector2 p_size) { _size = p_size; }
	void set_type(Type p_type) { _type = p_type; }
	void set_move_speed(jgl::Ulong p_move_speed) { _move_speed = p_move_speed; }

	Type type() { return (_type); }
	jgl::Long id() { return (_id); }
	jgl::Vector2 pos() const { return (_pos); }
	jgl::Vector2 size() const { return (_size); }
	jgl::Vector2 direction() const { return (_direction); }
	jgl::Bool is_moving() { return (_is_moving); }
};