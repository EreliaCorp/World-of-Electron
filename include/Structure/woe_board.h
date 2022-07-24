#pragma once

#include "jgl.h"
#include "woe_network.h"

using Node = jgl::INode;
class Chunk : public jgl::IBakableChunk<Node, 32>
{
private:


public:
	Chunk(jgl::Vector2Int p_pos);

	void push(Message& p_msg);
	void pull(Message& p_msg);

	void randomize();
};

class Board : public jgl::Finite_tilemap<Chunk, 15, 15>
{
public:
	static const jgl::Size_t C_NB_PASS = 3;
	static const jgl::Size_t C_SIZE = 15;
	jgl::Bool _placed_areas[C_SIZE][C_SIZE];

private:
	jgl::Vector2Int _calc_next_space(jgl::Vector2Int p_pos);
	void _place_next_sub_area(jgl::Vector2Int p_pos, jgl::Vector2Int p_size);
	jgl::Size_t _count_neightbour(jgl::Vector2Int p_pos);
	void _smooth();
	void _randomize();
	void _place_spawns();
	void _place_areas();

	static jgl::Vector2Int _direction_value[8];
	static jgl::Vector2Int _axis_direction_value[4];
	static jgl::Int _direction_mask[8];
	static jgl::Int _rev_direction_mask[8];

public:
	Board();
	void generate();

	void push(Message& p_msg);
	void pull(Message& p_msg);

	jgl::Bool can_acces(jgl::Vector2 p_pos);
};