#pragma once

#include "woe_entity.h"

class Player : public Entity
{
public:
	enum class Team
	{
		Red,
		Green,
		Blue
	};

private:

	Team _team;

public:
	Player(Team p_team, jgl::Long id) : Entity(Type::Player, id),
		_team(p_team)
	{

	}

	Team team() { return (_team); }
	void set_team(Team p_team) { _team = p_team; }

	virtual void update(jgl::Ulong p_ticks)
	{
		update_pos(p_ticks);
	}
};