#pragma once

#include "jgl.h"

class Entity
{
public:
	enum class Type
	{
		Grey = 0,
		Red = 1,
		Green = 2,
		Blue = 3
	};
protected:
	Type _type;
	jgl::Long _id;
	jgl::Bool _is_moving;
	jgl::Vector2 _pos;

	//Movement relative attribut
	jgl::Vector2 _starting_pos;
	jgl::Vector2 _destination;
	jgl::Vector2 _movement;
	jgl::Ulong _move_speed;
	jgl::Ulong _start_motion_time;

	void _join_destination();
public:
	Entity(Type p_type, jgl::Long p_id);
	virtual void place(jgl::Vector2 p_pos);
	void move(jgl::Vector2 p_delta);

	void render(jgl::Vector2Int p_anchor, jgl::Vector2Int p_size, jgl::Float p_depth);

	void set_id(jgl::Long p_id) { _id = p_id; }
	void set_type(Type p_type) { _type = p_type; }
	void set_move_speed(jgl::Ulong p_move_speed) { _move_speed = p_move_speed; }

	virtual void update();
	void update_pos();

	Type type() { return (_type); }
	jgl::Long id() { return (_id); }
	jgl::Vector2 pos() const { return (_pos); }
	jgl::Vector2 destination() const { return (_destination); }
	jgl::Vector2 movement() const { return (_movement); }

	jgl::Bool is_moving() { return (_is_moving); }
};