#pragma once

#include "jgl.h"

#include "widget/woe_abstract_manager.h"

#include "Utils/woe_configuration_file.h"

#include "structure/woe_board.h"
#include "widget/woe_board_manager.h"
#include "widget/woe_player_manager.h"
#include "widget/woe_entity_manager.h"

class Main_application : public jgl::Singleton_widget<Main_application>, public Abstract_manager
{
public:
	friend class jgl::Singleton_widget<Main_application>;
	
	enum class State
	{
		Loading,
		Connection,
		Player_instantiation,
		Board_instantiation,
		Idle
	};
	using State_machine = jgl::State_machine<State>;

private:
	Main_application(jgl::Widget* p_parent);

	State_machine _state_machine;

	Configuration_file _config_file;

	void _render();
	void _on_geometry_change();

	jgl::Bool _update();
	jgl::Bool _fixed_update();

	void _initiate_server();
	void _initiate_client();

	Board_manager* _board_manager;
	Entity_manager* _entity_manager;
	Player_manager* _player_manager;

public:

};