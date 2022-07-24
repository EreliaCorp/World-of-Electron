#pragma once

#include "woe_entity.h"

class Projectile : public Entity
{
private:
	jgl::Long _source_id;

public:
	static inline jgl::Bool update_bool = true;

	Projectile(jgl::Long id, jgl::Long p_source_id, jgl::Vector2 p_direction) : Entity(Type::Projectile, id),
		_source_id(p_source_id)
	{
		_size = 0.1f;
		move(p_direction);
	}

	void update(jgl::Ulong p_ticks)
	{
		update_pos(p_ticks);
		if (_is_moving == false)
			Engine::instance()->remove_entity(this);
	}

	jgl::Long source_id() { return (_source_id); }
	void set_source_id(jgl::Long p_source_id) { _source_id = p_source_id; }
};