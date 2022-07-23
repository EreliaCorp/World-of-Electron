#include "widget/woe_player_manager.h"
#include "woe_network.h"
#include "structure/woe_engine.h"

void Player_manager::_render()
{
	
}

void Player_manager::_on_geometry_change()
{
	Engine::instance()->board()->unbake();
}

void Player_manager::_push_motion_command(jgl::Vector2Int p_delta)
{
	Message msg(Server_message::Player_movement_command);

	msg << Engine::instance()->player()->id();
	msg << p_delta;

	_motion_command_send = true;

	Client_manager::client()->send(msg);
}

void Player_manager::_pull_motion_command(Message& p_msg)
{
	jgl::Long id;
	jgl::Vector2Int delta;

	p_msg >> id;
	p_msg >> delta;

	Entity* tmp_entity = Engine::instance()->entity(id);

	if (tmp_entity != nullptr && tmp_entity->is_moving() == false)
	{
		jgl::Vector2Int result = 0;

		jgl::Vector2Int delta_value[2] = { jgl::Vector2Int(1, 0), jgl::Vector2Int(0, 1) };

		for (jgl::Size_t i = 0; i < 2; i++)
		{
			if (Engine::instance()->board()->can_acces(tmp_entity->pos(), delta * delta_value[i]) == true &&
				Engine::instance()->entity_collision(tmp_entity, tmp_entity->pos() + delta * delta_value[i]) == nullptr)
			{
				result += delta * delta_value[i];
			}
		}

		if (result != delta || (Engine::instance()->board()->can_acces(tmp_entity->pos(), delta) == true &&
			Engine::instance()->entity_collision(tmp_entity, tmp_entity->pos() + delta) == nullptr))
		{
			tmp_entity->move(result);
		}
	}
}

jgl::Bool Player_manager::_update()
{
	if (_motion_command_send == false)
	{
		jgl::Key keys[4] = { jgl::Key::Z, jgl::Key::Q, jgl::Key::S, jgl::Key::D };
		jgl::Vector2Int delta_value[4] = { jgl::Vector2(0, -1) , jgl::Vector2(-1, 0) , jgl::Vector2(0, 1) , jgl::Vector2(1, 0) };
		jgl::Vector2Int delta_pos = 0;

		for (jgl::Size_t i = 0; i < 4; i++)
			if (jgl::Application::active_application()->keyboard().get_key(keys[i]) == jgl::Input_status::Down)
				delta_pos += delta_value[i];

		if (delta_pos != 0 && Engine::instance()->player()->is_moving() == false)
			_push_motion_command(delta_pos);
	}


	if (jgl::Application::active_application()->keyboard().get_key(jgl::Key::Key_1) == jgl::Input_status::Release)
	{
		Node::SIZE /= 2;
		Engine::instance()->board()->unbake();
	}

	if (jgl::Application::active_application()->keyboard().get_key(jgl::Key::Key_3) == jgl::Input_status::Release)
	{
		Node::SIZE *= 2;
		Engine::instance()->board()->unbake();
	}

	return (false);
}

jgl::Bool Player_manager::_fixed_update()
{
	return (false);
}

void Player_manager::_initiate_server()
{
	SERVER_ACTIVITY(Server_message::Player_id) {
		THROW_INFORMATION("Message received : Player information request");
		p_msg.clear();
		jgl::Long id = Engine::instance()->request_id();

		p_msg << id;

		Entity* new_entity = new Entity(Entity::Type::Grey, id);
		new_entity->place(Board::C_SIZE * Chunk::C_SIZE / 2);
		Engine::instance()->add_entity(new_entity);

		p_client->send(p_msg);
	});

	SERVER_ACTIVITY(Server_message::Player_movement_command) {
		_pull_motion_command(p_msg);
		Server_manager::server()->send_to(p_client, Server_message::Player_movement_treated);
	});
}

void Player_manager::_initiate_client()
{
	CLIENT_ACTIVITY(Server_message::Player_id) {
		THROW_INFORMATION("Message received : Player information result");
		jgl::Long id;

		p_msg >> id;
		Engine::instance()->set_player_id(id);
	});

	CLIENT_ACTIVITY(Server_message::Player_movement_treated) {
		_motion_command_send = false;
	});
}

Player_manager::Player_manager(jgl::Widget* p_parent) : jgl::Widget(p_parent), Abstract_manager(p_parent), IWidget(p_parent)
{
	Chunk::set_node_texture(new jgl::Sprite_sheet("ressource/texture/wall_spritesheet.png", jgl::Vector2Int(24, 12)));
}