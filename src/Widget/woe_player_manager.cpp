#include "widget/woe_player_manager.h"
#include "woe_network.h"
#include "structure/woe_engine.h"
#include "widget/woe_debug_screen.h"

#include "structure/woe_projectile.h"

Player_manager::Player_manager(jgl::Widget* p_parent) : jgl::Widget(p_parent), Abstract_manager(p_parent), IWidget(p_parent),
_motion_command_treated(true),
_shoot_command_treated(true),
_shoot_timer(250)
{
	Chunk::set_node_texture(new jgl::Sprite_sheet("ressource/texture/wall_spritesheet.png", jgl::Vector2Int(24, 12)));
}

void Player_manager::_render()
{
	
}

void Player_manager::_on_geometry_change()
{
	Engine::instance()->board()->unbake();
}

void Player_manager::_push_motion_command(jgl::Vector2Int p_delta)
{
	Message msg(Server_message::Player_movement);

	msg << Engine::instance()->player()->id();
	msg << p_delta;

	_motion_command_treated = false;

	Client_manager::client()->send(msg);
}

void Player_manager::_pull_motion_command(Message& p_msg)
{
	jgl::Long id;
	jgl::Vector2Int delta;

	p_msg >> id;
	p_msg >> delta;

	Entity* tmp_entity = Engine::instance()->entity(id);

	if (tmp_entity != nullptr)
	{
		jgl::Vector2 norm = jgl::Vector2(delta).normalize();

		tmp_entity->move(norm);
	}
}

void Player_manager::_push_shoot_command(jgl::Long p_source_id, jgl::Vector2 p_origin, jgl::Vector2 p_direction)
{
	Message msg(Server_message::Cast_shoot);

	msg << p_source_id;
	msg << p_origin;
	msg << p_direction;

	Client_manager::client()->send(msg);
}

void Player_manager::_pull_shoot_command(Message& p_msg)
{
	jgl::Long source_id;
	jgl::Vector2 origin;
	jgl::Vector2 direction;

	p_msg >> source_id;
	p_msg >> origin;
	p_msg >> direction;

	Engine::instance()->cast_shoot(source_id, origin, direction);
}

jgl::Bool Player_manager::_update()
{
	if (_motion_command_treated == true)
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

	if (_shoot_command_treated == true && _shoot_timer.timeout() == true)
	{
		if (jgl::Application::active_application()->mouse().get_button(jgl::Mouse_button::Left) == jgl::Input_status::Down)
		{
			jgl::Vector2 origin = Engine::instance()->player()->pos();
			jgl::Vector2 direction = (_convert_screen_to_world(jgl::Application::active_application()->mouse().pos()) - origin).normalize();

			_push_shoot_command(Engine::instance()->player_id(), origin, direction);
			_shoot_timer.start();
		}
	}


	if (jgl::Application::active_application()->keyboard().get_key(jgl::Key::F) == jgl::Input_status::Release)
	{
		Projectile::update_bool = !Projectile::update_bool;
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

	if (Debug_screen::instance()->update_timer().timeout() == true)
	{
		if (Engine::instance()->player() != nullptr)
			Debug_screen::instance()->set_text("Player pos : " + Engine::instance()->player()->pos().text(), 1, 0);
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

		Entity* new_entity = new Entity(Entity::Type::NPC, id);
		jgl::Vector2 pos = Board::C_SIZE * Chunk::C_SIZE / 2 + 0.5f;
		while (Engine::instance()->board() != nullptr && Engine::instance()->board()->can_acces(pos) == false)
			pos.x -= 1;
		new_entity->place(pos);

		Engine::instance()->add_entity(new_entity);

		p_client->send(p_msg);
	});

	SERVER_ACTIVITY(Server_message::Player_movement) {
		_pull_motion_command(p_msg);
		Server_manager::server()->send_to(p_client, Server_message::Player_movement);
	});

	SERVER_ACTIVITY(Server_message::Cast_shoot) {
		_pull_shoot_command(p_msg);
		Server_manager::server()->send_to(p_client, Server_message::Cast_shoot);
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

	CLIENT_ACTIVITY(Server_message::Player_movement) {
		_motion_command_treated = true;
	});

	CLIENT_ACTIVITY(Server_message::Cast_shoot) {
		_shoot_command_treated = true;
	});
}