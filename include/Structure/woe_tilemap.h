#pragma once

#include "jgl.h"
#include "woe_network.h"

using Node = jgl::INode;
class Chunk : public jgl::IBakableChunk<Node, 15>
{
private:


public:
	Chunk(jgl::Vector2Int p_pos);
	void randomize();
};

class Tilemap : public jgl::Finite_tilemap<Chunk, 15, 15>
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
	void _place_areas();

public:
	Tilemap();
	Message upload();
	void download(Message& p_msg);
	void generate();
};