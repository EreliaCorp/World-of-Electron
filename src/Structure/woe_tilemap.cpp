#include "structure/woe_tilemap.h"


void Chunk::randomize()
{
	for (jgl::Size_t i = 0; i < Chunk::C_SIZE; i++)
	{
		for (jgl::Size_t j = 0; j < Chunk::C_SIZE; j++)
		{
			this->set_content(jgl::Vector2Int(i, j), jgl::generate_nbr(0, 2) % 2 == 0 ? -1 : 0);
		}
	}
}

Chunk::Chunk(jgl::Vector2Int p_pos) : jgl::IBakableChunk<Node, Chunk::C_SIZE, Chunk::C_DEPTH>(p_pos)
{

}

jgl::Vector2Int Tilemap::_calc_next_space(jgl::Vector2Int p_pos)
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

void Tilemap::_place_next_sub_area(jgl::Vector2Int p_pos, jgl::Vector2Int p_size)
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

jgl::Size_t Tilemap::_count_neightbour(jgl::Vector2Int p_pos)
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

void Tilemap::_smooth()
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

void Tilemap::_randomize()
{
	for (jgl::Size_t i = 0; i < C_SIZE; i++)
	{
		for (jgl::Size_t j = 0; j < C_SIZE; j++)
		{
			this->chunk(jgl::Vector2Int(i, j))->randomize();
		}
	}
}

void Tilemap::_place_areas()
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

void Tilemap::generate()
{
	_randomize();

	_place_areas();

	for (jgl::Size_t i = 0; i < C_NB_PASS; i++)
	{
		_smooth();
	}
}

Tilemap::Tilemap()
{
	for (jgl::Size_t i = 0; i < C_SIZE; i++)
	{
		for (jgl::Size_t j = 0; j < C_SIZE; j++)
		{
			_chunks[i][j] = new Chunk(jgl::Vector2Int(i, j));
		}
	}
}