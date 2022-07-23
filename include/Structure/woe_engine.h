#pragma once

#include "jgl.h"

#include "structure/woe_board.h"
#include "structure/woe_entity.h"

class Engine : public jgl::Singleton<Engine>
{
	friend jgl::Singleton<Engine>;

private:
	Board* _board;

	jgl::Map<jgl::Long, Entity*> _entities;

	jgl::Long _player_id;
	Entity* _player;

	Engine();

public:
	void update();

	jgl::Long request_id();

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

	void add_entity(Entity* p_entity)
	{
		if (_entities.count(p_entity->id()) != 0)
			delete _entities[p_entity->id()];
		_entities[p_entity->id()] = p_entity;
	}

	void remove_entity(Entity* p_entity)
	{
		auto tmp = _entities.find(p_entity->id());

		if (tmp != _entities.end())
			_entities.erase(tmp);
	}

	jgl::Map<jgl::Long, Entity*>& entities()
	{
		return (_entities);
	}
	Entity* entity(jgl::Long p_id)
	{
		if (_entities.count(p_id) == 0)
			return (nullptr);
		return (_entities[p_id]);
	}
};