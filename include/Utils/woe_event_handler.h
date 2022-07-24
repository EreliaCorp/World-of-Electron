#pragma once

#include "jgl.h"

enum class Event
{
	Start_loading,
	Start_connection,
	Configure_server_interaction,
	Configure_client_interaction,
	Start_player_instantiation,
	Start_board_instantiation,
	Resize_manager_widget,
	Activating_manager_widget,
	Desactivating_manager_widget,
	Start_game
};

class Event_handler : public jgl::Singleton<jgl::Publisher<Event>>
{
	friend jgl::Singleton<jgl::Publisher<Event>>;
public:
	static void notify(Event p_event)
	{
		instance()->notify(p_event);
	}
	static void subscribe(Event p_event, jgl::Publisher<Event>::Activity p_funct)
	{
		instance()->subscribe(p_event, p_funct);

	}
};