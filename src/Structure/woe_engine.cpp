#include "structure/woe_engine.h"
#include "structure/woe_projectile.h"
#include "Utils/woe_event_handler.h"

Engine::Engine() :
	_board(nullptr)
{
	set_player_id(-1);
}

void Engine::remove_entities()
{
	if (_entities_to_delete.size() == 0)
		return;

	lock_entities_mutex();

	Message msg(Server_message::Delete_entity);

	for (jgl::Size_t i = 0; i < _entities_to_delete.size(); i++)
	{
		jgl::Long key = _entities_to_delete[i];
		if (_entities.count(key) != 0)
		{
			if (Server_manager::instance() != nullptr)
				msg << key;
			if (_player->id() == key)
				_player = nullptr;
			delete _entities[key];
			_entities.erase(key);
		}
	}
	unlock_entities_mutex();

	if (Server_manager::instance() != nullptr)
		Server_manager::instance()->send_to_all(msg);

	_entities_to_delete.clear();
}

void Engine::update(jgl::Ulong p_ticks)
{
	for (auto tmp : _entities)
	{
		tmp.second->update(p_ticks);
	}
}

void Engine::add_entity(Entity* p_entity)
{
	if (p_entity != nullptr)
	{
		if (_entities.count(p_entity->id()) != 0)
			delete _entities[p_entity->id()];
		_entities[p_entity->id()] = p_entity;
	}
}

void Engine::remove_entity(Entity* p_entity)
{
	_entities_to_delete.push_back(p_entity->id());
}
void Engine::remove_entity(jgl::Long p_id)
{
	_entities_to_delete.push_back(p_id);
}

jgl::Long Engine::request_id()
{
	jgl::Long result = 0;
	for (; _entities.count(result) != 0 || _entities[result] != nullptr; result++) {}
	return (result);
}

void Engine::cast_shoot(jgl::Long source_id, jgl::Vector2 p_origin, jgl::Vector2 p_direction)
{
	Projectile* new_projectile = new Projectile(request_id(), source_id, p_direction);
	new_projectile->place(p_origin);
	add_entity(new_projectile);
}

Entity* Engine::entity_collision(Entity* p_entity, jgl::Vector2Int p_pos)
{
	for (auto tmp : _entities)
	{
		Entity* tmp_entity = tmp.second;
		if (p_entity != nullptr && tmp_entity != p_entity &&
			tmp_entity->type() != Entity::Type::Projectile &&
			tmp_entity->pos().distance(p_pos) < 1.0f)
		{
			return (tmp_entity);
		}
	}

	return (nullptr);
}