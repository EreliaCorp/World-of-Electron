#include "structure/woe_projectile.h"
#include "structure/woe_engine.h"

Projectile::Projectile(jgl::Long id, jgl::Long p_source_id, jgl::Vector2 p_direction) : Entity(Type::Projectile, id),
_source_id(p_source_id)
{
	_move_speed = 100;
	_size = 0.2f;
	move(p_direction);
}

void Projectile::update(jgl::Ulong p_ticks)
{
	update_pos(p_ticks);
	if (_is_moving == false)
		Engine::instance()->remove_entity(this);
}