#pragma once

#include "jgl.h"

#include "Utils/woe_configuration_file.h"

#include "structure/woe_board.h"
#include "widget/woe_board_manager.h"
#include "widget/woe_player_manager.h"
#include "widget/woe_entity_manager.h"

class Main_application : public jgl::Singleton_widget<Main_application>, public jgl::Widget
{
public:
	friend class jgl::Singleton_widget<Main_application>;
	
	enum class State
	{
		Loading,
		Connection,
		Player_instantiation,
		Board_instantiation,
		In_game
	};
	jgl::String to_string(State p_state)
	{
		switch (p_state)
		{
		case (State::Loading):
			return ("Loading");
		case (State::Connection):
			return ("Connection");
		case (State::Player_instantiation):
			return ("Player instantiation");
		case (State::Board_instantiation):
			return ("Board instantiation");
		case (State::In_game):
			return ("In game");
		default:
			return ("Undefined");
		}
	}
	using State_machine = jgl::State_machine<State>;

private:
	Main_application(jgl::Widget* p_parent);

	State_machine _state_machine;

	Configuration_file _config_file;

	void _render();
	void _on_geometry_change();

	jgl::Bool _update();
	jgl::Bool _fixed_update();

	void _set_state(State p_state);

	Board_manager* _board_manager;
	Entity_manager* _entity_manager;
	Player_manager* _player_manager;

public:

};