#include "widget/woe_entity_manager.h"
#include "widget/woe_debug_screen.h"
#include "structure/woe_engine.h"
#include "structure/woe_player.h"
#include "structure/woe_projectile.h"

void Entity_manager::_render()
{
	Engine::instance()->lock_entities_mutex();
	for (auto tmp : Engine::instance()->entities())
	{
		if (tmp.second != nullptr)
		{
			jgl::Vector2Int anchor = _convert_world_to_screen(tmp.second->pos());
			jgl::Float depth = _depth + Chunk::C_DEPTH / 2 + 0.5f;

			tmp.second->render(anchor, Node::SIZE, depth);
		}
	}
	Engine::instance()->unlock_entities_mutex();
}

void Entity_manager::_on_geometry_change()
{

}

jgl::Bool Entity_manager::_update()
{
	if (_entity_updater_timer.timeout() == true)
	{
		_entity_updater_timer.start();

		jgl::Ulong actual_tick = jgl::Application::active_application()->time();
		jgl::Ulong delta_tick = actual_tick - _last_tick;

		Debug_screen::instance()->set_text("Nb of entities : " + jgl::itoa(Engine::instance()->entities().size()), 1, 1);

		Engine::instance()->remove_entities();

		if (Server_manager::instance() != nullptr)
		{
			Engine::instance()->update(delta_tick);

			_push_entity_update();
		}

		_last_tick = actual_tick;
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

		Entity* tmp_entity = Engine::instance()->entity(id);

		if (tmp_entity == nullptr && _asked_entities[id] == false)
		{
			_asked_entities[id] = true;
			result << id;
		}
		else
		{
			tmp_entity->place(pos);
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
			_upload_entity(result, tmp_entity);
		}
	}

	if (result.empty() == false)
		Server_manager::server()->send_to(p_client, result);
}

void Entity_manager::_pull_entity_data(Message& p_msg)
{
	while (p_msg.empty() == false)
	{
		Entity* result = _download_entity(p_msg);
		_asked_entities[result->id()] = false;
		if (result != nullptr)
			Engine::instance()->add_entity(result);
	}
}

void Entity_manager::_pull_entity_suppression(Message& p_msg)
{
	while (p_msg.empty() == false)
	{
		jgl::Long id;

		p_msg >> id;

		Engine::instance()->remove_entity(id);
	}
}

jgl::Bool Entity_manager::_fixed_update()
{
	return (false);
}

void Entity_manager::_upload_entity(Message& p_msg, Entity* p_entity)
{
	p_msg << p_entity->id();
	p_msg << p_entity->type();
	p_msg << p_entity->pos();
	switch (p_entity->type())
	{
		case Entity::Type::NPC:
		{

			break;
		}
		case Entity::Type::Object:
		{

			break;
		}
		case Entity::Type::Player:
		{
			p_msg << static_cast<Player*>(p_entity)->team();
			break;
		}
		case Entity::Type::Projectile:
		{
			p_msg << static_cast<Projectile*>(p_entity)->source_id();
			p_msg << p_entity->direction();
			break;
		}
	}
}

Entity* Entity_manager::_download_entity(Message& p_msg)
{
	jgl::Long id;
	Entity::Type type;
	jgl::Vector2 pos;

	p_msg >> id;
	p_msg >> type;
	p_msg >> pos;

	Entity* result = nullptr;

	switch (type)
	{
	case Entity::Type::NPC:
	{
		result = new Entity(type, id);
		break;
	}
	case Entity::Type::Object:
	{
		result = new Entity(type, id);
		break;
	}
	case Entity::Type::Player:
	{
		Player::Team team;

		p_msg >> team;

		result = new Player(team, id);

		break;
	}
	case Entity::Type::Projectile:
	{
		jgl::Long source_id;
		jgl::Vector2 direction;

		p_msg >> source_id;
		p_msg >> direction;

		result = new Projectile(id, source_id, direction);

		break;
	}
	}

	if (result == nullptr)
		return (nullptr);

	result->place(pos);

	return (result);
}

void Entity_manager::_initiate_server()
{
	SERVER_ACTIVITY(Server_message::Entity_data) {
		_push_entity_data(p_client, p_msg);
	});
}

void Entity_manager::_initiate_client()
{
	CLIENT_ACTIVITY(Server_message::Update_entity_data) {
		if (Server_manager::instance() == nullptr)
			_pull_entity_update(p_msg);
	});

	CLIENT_ACTIVITY(Server_message::Entity_data) {
		_pull_entity_data(p_msg);
	});
	
	CLIENT_ACTIVITY(Server_message::Delete_entity) {
		if (Server_manager::instance() == nullptr)
			_pull_entity_suppression(p_msg);
	});
}

Entity_manager::Entity_manager(jgl::Widget* p_parent) : jgl::Widget(p_parent), Abstract_manager(p_parent), IWidget(p_parent),
	_last_tick(jgl::Application::active_application()->time()),
	_entity_updater_timer(15)
{

}