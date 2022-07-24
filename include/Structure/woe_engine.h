#pragma once

#include "jgl.h"

#include "structure/woe_board.h"
#include "structure/woe_entity.h"

class Engine : public jgl::Singleton<Engine>
{
	friend jgl::Singleton<Engine>;

private:
	Board* _board;

	std::mutex _entities_mutex;
	jgl::Map<jgl::Long, Entity*> _entities;

	jgl::Array<jgl::Long> _entities_to_delete;

	jgl::Long _player_id;
	Entity* _player;

	Engine();

public:
	void remove_entities();
	void update(jgl::Ulong p_ticks);

	jgl::Long request_id();

	void cast_shoot(jgl::Long source_id, jgl::Vector2 p_origin, jgl::Vector2 p_direction);

	Entity* entity_collision(Entity* p_entity, jgl::Vector2Int p_pos);

	void create_board(){ _board = new Board(); }
	Board* board() { return (_board); }
	
	Entity* player()
	{
		if (_player == nullptr)
		{
			auto tmp = _entities.find(_player_id);

			if (tmp != _entities.end())
				_player = tmp->second;
		}
		return (_player);
	}
	void set_player_id(jgl::Long p_player_id)
	{
		_player = nullptr;
		_player_id = p_player_id;
	}
	jgl::Long player_id() { return (_player_id); }

	void add_entity(Entity* p_entity);
	void remove_entity(Entity* p_entity);

	jgl::Map<jgl::Long, Entity*>& entities()
	{
		return (_entities);
	}
	Entity* entity(jgl::Long p_id)
	{
		if (_entities.count(p_id) == 0)
		{
			return (nullptr);
		}
		return (_entities[p_id]);
	}
};