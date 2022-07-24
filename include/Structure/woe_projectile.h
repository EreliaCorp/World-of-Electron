#pragma once

#include "woe_entity.h"

class Projectile : public Entity
{
private:
	jgl::Long _source_id;

public:
	static inline jgl::Bool update_bool = true;

	Projectile(jgl::Long id, jgl::Long p_source_id, jgl::Vector2 p_direction);

	void update(jgl::Ulong p_ticks);

	jgl::Long source_id() { return (_source_id); }
	void set_source_id(jgl::Long p_source_id) { _source_id = p_source_id; }
};