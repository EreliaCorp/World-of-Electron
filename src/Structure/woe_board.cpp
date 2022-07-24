#include "structure/woe_board.h"

#include <bitset>

jgl::Vector2Int Board::_direction_value[8] = {
	jgl::Vector2Int(-1, -1),
	jgl::Vector2Int(0, -1),
	jgl::Vector2Int(1, -1),
	jgl::Vector2Int(1, 0),
	jgl::Vector2Int(-1, 0),
	jgl::Vector2Int(-1, 1),
	jgl::Vector2Int(0, 1),
	jgl::Vector2Int(1, 1),
};
jgl::Vector2Int Board::_axis_direction_value[4] = {
	jgl::Vector2Int(0, -1),
	jgl::Vector2Int(1, 0),
	jgl::Vector2Int(-1, 0),
	jgl::Vector2Int(0, 1)
};
jgl::Int Board::_direction_mask[8] = {
	Node::NORTH_WALKABLE | Node::WEST_WALKABLE,
	Node::NORTH_WALKABLE,
	Node::NORTH_WALKABLE | Node::EAST_WALKABLE,
	Node::EAST_WALKABLE,
	Node::WEST_WALKABLE,
	Node::SOUTH_WALKABLE | Node::WEST_WALKABLE,
	Node::SOUTH_WALKABLE,
	Node::SOUTH_WALKABLE | Node::EAST_WALKABLE
};

jgl::Int Board::_rev_direction_mask[8] = {
	Node::SOUTH_WALKABLE | Node::EAST_WALKABLE,
	Node::SOUTH_WALKABLE,
	Node::SOUTH_WALKABLE | Node::WEST_WALKABLE,
	Node::WEST_WALKABLE,
	Node::EAST_WALKABLE,
	Node::NORTH_WALKABLE | Node::EAST_WALKABLE,
	Node::NORTH_WALKABLE,
	Node::NORTH_WALKABLE | Node::WEST_WALKABLE
};

jgl::Vector2Int Board::_calc_next_space(jgl::Vector2Int p_pos)
{
	jgl::Vector2Int max_size = 0;

	for (; max_size.x < 4 && p_pos.x + max_size.x < C_SIZE &&
		_placed_areas[p_pos.x + max_size.x][p_pos.y] == false &&
		_placed_areas[p_pos.x + max_size.x][p_pos.y + max_size.y] == false; max_size.x++);
	for (; max_size.y < 4 && p_pos.y + max_size.y < C_SIZE &&
		_placed_areas[p_pos.x][p_pos.y + max_size.y] == false &&
		_placed_areas[p_pos.x + max_size.x][p_pos.y + max_size.y] == false; max_size.y++);

	return (jgl::Vector2Int(
		jgl::generate_nbr(1, max_size.x),
		jgl::generate_nbr(1, max_size.y)
	));
}

void Board::_place_next_sub_area(jgl::Vector2Int p_pos, jgl::Vector2Int p_size)
{
	for (jgl::Size_t x = 0; x < p_size.x; x++)
	{
		for (jgl::Size_t y = 0; y < p_size.y; y++)
		{
			_placed_areas[p_pos.x + x][p_pos.y + y] = true;
		}
	}

	for (jgl::Size_t x = 0; x < p_size.x * Chunk::C_SIZE; x++)
	{
		for (jgl::Size_t y = 0; y < p_size.y * Chunk::C_SIZE; y++)
		{
			if (x == 0 || y == 0 || x == p_size.x * Chunk::C_SIZE - 1 || y == p_size.y * Chunk::C_SIZE - 1)
				this->set_content(jgl::Vector2Int(p_pos.x * Chunk::C_SIZE + x, p_pos.y * Chunk::C_SIZE + y), 0);
		}
	}
}

jgl::Size_t Board::_count_neightbour(jgl::Vector2Int p_pos)
{
	static jgl::Vector2Int delta[9] = {
		jgl::Vector2Int(-1, -1),
		jgl::Vector2Int(0, -1),
		jgl::Vector2Int(1, -1),
		jgl::Vector2Int(-1, 0),
		jgl::Vector2Int(0, 0),
		jgl::Vector2Int(1, 0),
		jgl::Vector2Int(-1, 1),
		jgl::Vector2Int(0, 1),
		jgl::Vector2Int(1, 1),
	};

	jgl::Size_t result = 0;

	for (jgl::Size_t i = 0; i < 9; i++)
	{
		if (this->content(p_pos + delta[i]) != -1)
			result++;
	}

	return (result);
}

void Board::_smooth()
{
	for (jgl::Size_t i = 0; i < C_SIZE * Chunk::C_SIZE; i++)
	{
		for (jgl::Size_t j = 0; j < C_SIZE * Chunk::C_SIZE; j++)
		{
			jgl::Size_t neightbour = _count_neightbour(jgl::Vector2Int(i, j));
			jgl::Short actual = this->content(jgl::Vector2Int(i, j));

			if (actual == -1)
			{
				if (neightbour > 5)
				{
					this->set_content(jgl::Vector2Int(i, j), 0);
				}
			}
			else
			{
				if (neightbour < 5)
				{
					this->set_content(jgl::Vector2Int(i, j), -1);
				}
			}
		}
	}
}

void Board::_randomize()
{
	for (jgl::Size_t i = 0; i < C_SIZE; i++)
	{
		for (jgl::Size_t j = 0; j < C_SIZE; j++)
		{
			_placed_areas[i][j] = false;
			this->chunk(jgl::Vector2Int(i, j))->randomize();
		}
	}
}

void Board::_place_spawns()
{

}

void Board::_place_areas()
{
	for (jgl::Size_t i = 0; i < C_SIZE; i++)
	{
		for (jgl::Size_t j = 0; j < C_SIZE; j++)
		{
			if (_placed_areas[i][j] == false)
			{
				jgl::Vector2Int pos = jgl::Vector2Int(i, j);
				jgl::Vector2Int size = _calc_next_space(pos);

				_place_next_sub_area(pos, size);
			}
		}
	}
}

void Board::generate()
{
	_randomize();

	_place_spawns();
	_place_areas();

	for (jgl::Size_t i = 0; i < C_NB_PASS; i++)
	{
		_smooth();
	}

	unbake();
}

Board::Board()
{
	for (jgl::Size_t i = 0; i < C_SIZE; i++)
	{
		for (jgl::Size_t j = 0; j < C_SIZE; j++)
		{
			_chunks[i][j] = new Chunk(jgl::Vector2Int(i, j));
		}
	}
}

void Board::push(Message& p_msg)
{
	for (jgl::Size_t i = 0; i < C_SIZE; i++)
	{
		for (jgl::Size_t j = 0; j < C_SIZE; j++)
		{
			_chunks[i][j]->push(p_msg);
		}
	}
}

void Board::pull(Message& p_msg)
{
	for (jgl::Size_t i = 0; i < C_SIZE; i++)
	{
		for (jgl::Size_t j = 0; j < C_SIZE; j++)
		{
			_chunks[i][j]->pull(p_msg);
		}
	}
}

jgl::Bool Board::can_acces(jgl::Vector2 p_pos)
{
	if (content(p_pos, 0) != -1)
		return (false);
	return (true);
}