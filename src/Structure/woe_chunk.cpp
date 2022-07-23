#include "structure/woe_board.h"


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

void Chunk::push(Message& p_msg)
{
	p_msg << _content;
}

void Chunk::pull(Message& p_msg)
{
	p_msg >> _content;
}