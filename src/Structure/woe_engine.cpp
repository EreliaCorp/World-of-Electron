#include "structure/woe_engine.h"

Engine::Engine() :
	_board(nullptr)
{
	set_player_id(-1);
}

void Engine::update()
{
	for (auto tmp : _entities)
	{
		if (tmp.second->is_moving() == true)
		{
			tmp.second->update_pos();
		}
	}
}

jgl::Long Engine::request_id()
{
	jgl::Long result = 0;
	for (; _entities.count(result) != 0; result++) {}
	return (result);
}

Entity* Engine::entity_collision(Entity* p_entity, jgl::Vector2Int p_pos)
{
	for (auto tmp : _entities)
	{
		Entity* tmp_entity = tmp.second;
		if (p_entity != nullptr && tmp_entity != p_entity && tmp_entity->pos().distance(p_pos) < 1.0f)
			return (tmp_entity);
	}

	return (nullptr);
}