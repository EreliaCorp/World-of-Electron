#include "widget/woe_board_manager.h"
#include "woe_network.h"
#include "structure/woe_engine.h"

void Board_manager::_render()
{
	jgl::Vector2Int start = _convert_screen_to_chunk(0);
	jgl::Vector2Int end = _convert_screen_to_chunk(_area);

	jgl::Int animation_state = (jgl::Application::active_application()->time() % 1000) / 250;

	if (Node::SIZE < 4)
		animation_state = 0;

	for (jgl::Int x = start.x; x <= end.x; x++)
	{
		for (jgl::Int y = start.y; y <= end.y; y++)
		{
			jgl::Vector2Int chunk_pos = jgl::Vector2Int(x, y);
			Chunk* tmp_chunk = Engine::instance()->board()->request_chunk(chunk_pos);

			if (tmp_chunk != nullptr)
			{
				if (tmp_chunk->baked() == false)
					tmp_chunk->bake(Engine::instance()->board(), true);

				tmp_chunk->render(jgl::convert_screen_to_opengl(_convert_chunk_to_screen(chunk_pos), _depth), 0);
			}
		}
	}
}

void Board_manager::_on_geometry_change()
{
	Engine::instance()->board()->unbake();
}

jgl::Bool Board_manager::_update()
{
	return (false);
}

jgl::Bool Board_manager::_fixed_update()
{
	return (false);
}

void Board_manager::_initiate_server()
{
	SERVER_ACTIVITY(Server_message::Board_data) {
		THROW_INFORMATION("Message received : Board composition request");
		p_msg.clear();
		Engine::instance()->board()->push(p_msg);
		p_client->send(p_msg);
	});
}

void Board_manager::_initiate_client()
{
	CLIENT_ACTIVITY(Server_message::Board_data) {
		THROW_INFORMATION("Message received : Board composition result");
		if (Engine::instance()->board() != nullptr)
			delete Engine::instance()->board();
		Engine::instance()->create_board();
		Engine::instance()->board()->pull(p_msg);
	});
}

Board_manager::Board_manager(jgl::Widget* p_parent) : jgl::Widget(p_parent), Abstract_manager(p_parent), IWidget(p_parent)
{
	Chunk::set_node_texture(new jgl::Sprite_sheet("ressource/texture/wall_spritesheet.png", jgl::Vector2Int(24, 12)));
}