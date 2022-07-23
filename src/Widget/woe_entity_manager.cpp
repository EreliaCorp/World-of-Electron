#include "widget/woe_entity_manager.h"
#include "structure/woe_engine.h"

void Entity_manager::_render()
{
	for (std::pair<jgl::Long, Entity*> tmp : Engine::instance()->entities())
	{
		if (tmp.second != nullptr)
		{
			jgl::Vector2Int anchor = _convert_world_to_screen(tmp.second->pos());
			jgl::Float depth = _depth + Chunk::C_DEPTH / 2 + 0.5f;

			tmp.second->render(anchor, Node::SIZE, depth);
		}
	}
}

void Entity_manager::_on_geometry_change()
{

}

jgl::Bool Entity_manager::_update()
{
	if (Server_manager::instance() != nullptr)
	{
		if (_entity_updater_timer.timeout() == true)
		{
			_entity_updater_timer.start();

			Engine::instance()->update();

			_push_entity_update();
		}
	}

	return (false);
}

void Entity_manager::_push_entity_update()
{
	Message result(Server_message::Update_entity_data);

	for (std::pair<jgl::Long, Entity*> tmp : Engine::instance()->entities())
	{
		result << tmp.first;
		result << tmp.second->pos();
	}

	Server_manager::instance()->send_to_all(result, Server_manager::instance()->server()->self());
}

void Entity_manager::_pull_entity_update(Message& p_msg)
{
	Message result(Server_message::Entity_data);

	while (p_msg.empty() == false)
	{
		jgl::Long id;
		jgl::Vector2 pos;

		p_msg >> id;
		p_msg >> pos;

		if (Engine::instance()->entity(id) == nullptr &&
			_asked_entities[id] == false)
		{
			_asked_entities[id] = true;
			result << id;
		}
		else
		{
			Engine::instance()->entity(id)->place(pos);
		}
	}

	if (result.empty() == false)
		Client_manager::instance()->send(result);
}

void Entity_manager::_push_entity_data(Connection* p_client, Message& p_msg)
{
	Message result(Server_message::Entity_data);

	jgl::Long id;

	while (p_msg.empty() == false)
	{
		p_msg >> id;

		Entity* tmp_entity = Engine::instance()->entity(id);
		if (tmp_entity != nullptr)
		{
			result << tmp_entity->id();
			result << tmp_entity->type();
			result << tmp_entity->pos();
		}
	}

	if (result.empty() == false)
		Server_manager::server()->send_to(p_client, result);
}

void Entity_manager::_pull_entity_data(Message& p_msg)
{
	while (p_msg.empty() == false)
	{
		jgl::Long id;
		Entity::Type type;
		jgl::Vector2 pos;

		p_msg >> id;
		p_msg >> type;
		p_msg >> pos;

		Entity* new_entity = new Entity(type, id);
		new_entity->place(pos);

		Engine::instance()->add_entity(new_entity);
	}
}

jgl::Bool Entity_manager::_fixed_update()
{
	return (false);
}

void Entity_manager::_initiate_server()
{
	SERVER_ACTIVITY(Server_message::Entity_data) {
		_push_entity_data(p_client, p_msg);
	});
}

void Entity_manager::_initiate_client()
{
	if (Server_manager::instance() == nullptr)
	{
		CLIENT_ACTIVITY(Server_message::Update_entity_data) {
			_pull_entity_update(p_msg);
		});
	}
	CLIENT_ACTIVITY(Server_message::Entity_data) {
		_pull_entity_data(p_msg);
	});
}

Entity_manager::Entity_manager(jgl::Widget* p_parent) : jgl::Widget(p_parent), Abstract_manager(p_parent), IWidget(p_parent)
{

}