#include "Widget/woe_tilemap_manager.h"
#include "Utils/woe_event_handler.h"
#include "structure/woe_tilemap.h"

void Tilemap_manager::_render()
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
			Chunk* tmp_chunk = Engine::instance()->map()->request_chunk(chunk_pos);

			if (tmp_chunk != nullptr)
			{
				if (tmp_chunk->baked() == false)
					tmp_chunk->bake(Engine::instance()->map(), true);

				tmp_chunk->render(jgl::convert_screen_to_opengl(_convert_chunk_to_screen(chunk_pos), _depth), 0);
			}
		}
	}
}

void Tilemap_manager::_on_geometry_change()
{

}

jgl::Bool Tilemap_manager::_update()
{
	return (false);
}

jgl::Bool Tilemap_manager::_fixed_update()
{
	return (false);
}

void Tilemap_manager::_initialize_server()
{
	SERVER_ACTIVITY(Server_message::Request_map_content) {
		p_client->send(Engine::instance()->map()->upload());
	});
}

void Tilemap_manager::_initialize_client()
{
	CLIENT_ACTIVITY(Server_message::Map_content) {
		Engine::instance()->map()->download(p_msg);
	});
}

void Tilemap_manager::_load_map_data()
{
	Chunk::set_node_texture(new jgl::Sprite_sheet("ressource/texture/wall_spritesheet.png", jgl::Vector2Int(24, 12)));
	Chunk::add_node(new Node(0, jgl::Vector2Int(0, 0), true, Node::OBSTACLE, 1));
}

Tilemap_manager::Tilemap_manager(jgl::Widget* p_parent) : Manager(p_parent)
{
	_load_map_data();
}