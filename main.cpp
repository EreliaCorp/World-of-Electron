#include "jgl.h"

using Node = jgl::INode;
class Chunk : public jgl::IBakableChunk<Node, 16>
{
private:

public:
	Chunk(jgl::Vector2Int p_pos) : jgl::IBakableChunk<Node, Chunk::C_SIZE, Chunk::C_DEPTH>(p_pos)
	{
		jgl::cout << "Creating new chunk pos " << _pos << jgl::endl;
		for (jgl::Size_t i = 0; i < Chunk::C_SIZE; i++)
		{
			for (jgl::Size_t j = 0; j < Chunk::C_SIZE; j++)
			{
				if (i == 0 || j == 0)
					_content[i][j][0] = 0;
				else
				{
					_content[i][j][0] = -1;
				}
			}
		}
	}
};
using Finite_tilemap = jgl::Finite_tilemap<Chunk, 10, 10>;

class Player
{
private:
	jgl::Vector2Int _pos;

public:
	Player(jgl::Vector2Int p_pos)
	{
		_pos = p_pos;
	}

	jgl::Vector2Int pos()
	{
		return (_pos);
	}
};

class IWidget : public jgl::Widget
{
protected:
	Player* _player;

	jgl::Vector2Int _convert_world_to_screen(jgl::Vector2 p_pos)
	{
		jgl::Vector2Int result = p_pos * Node::SIZE + _area / 2;

		if (_player != nullptr)
			result -= _player->pos() * Node::SIZE;

		return (result);
	}

	jgl::Vector2Int _convert_chunk_to_screen(jgl::Vector2Int p_pos)
	{
		return (_convert_world_to_screen(p_pos * Chunk::C_SIZE));
	}

	jgl::Vector2 _convert_screen_to_world(jgl::Vector2Int p_pos)
	{
		jgl::Vector2 result = p_pos - _area / 2;
		result /= Node::SIZE;

		if (_player != nullptr)
			result += _player->pos();

		return (result.floor());
	}

	jgl::Vector2Int _convert_screen_to_chunk(jgl::Vector2Int p_pos)
	{
		jgl::Vector2 result = _convert_screen_to_world(p_pos);
		result /= Chunk::C_SIZE;
		return (result.floor());
	}
public:
	IWidget(Player* p_player, jgl::Widget* p_parent) : jgl::Widget(p_parent),
		_player(p_player)
	{
		
	}

};

class Map_manager : public IWidget
{
private:
	Finite_tilemap _map;

	void _render();
	void _on_geometry_change();

	jgl::Bool _update();
	jgl::Bool _fixed_update();

public:
	Map_manager(Player* p_player, jgl::Widget* p_parent);
};

void Map_manager::_render()
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
			Chunk* tmp_chunk = _map.request_chunk(chunk_pos);

			if (tmp_chunk != nullptr)
			{
				if (tmp_chunk->baked() == false)
					tmp_chunk->bake(&_map, true);

				tmp_chunk->render(jgl::convert_screen_to_opengl(_convert_chunk_to_screen(chunk_pos), _depth), 0);
			}
		}
	}

}

void Map_manager::_on_geometry_change()
{
	Node::SIZE = 4;
	_map.unbake();
}

jgl::Bool Map_manager::_update()
{
	return (false);
}

jgl::Bool Map_manager::_fixed_update()
{
	return (false);
}

Map_manager::Map_manager(Player* p_player, jgl::Widget* p_parent) : IWidget(p_player, p_parent)
{
	Chunk::set_node_texture(new jgl::Sprite_sheet("ressource/texture/wall_spritesheet.png", jgl::Vector2Int(24, 12)));

	Chunk::add_node(new Node(0, jgl::Vector2Int(0, 0), true, Node::OBSTACLE, 1));
}

int main()
{
	jgl::Application app("World of Electron", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.set_default_font("ressource/font/karma suture.ttf");
	app.active_multithread();

	Player player(jgl::Vector2Int(45, 60));

	Map_manager* manager = new Map_manager(&player, nullptr);
	manager->set_geometry(0, app.size());
	manager->activate();

	return (app.run());
}