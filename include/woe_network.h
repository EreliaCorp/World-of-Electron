#pragma once

#include "jgl.h"

enum class Server_message
{
	Player_id,
	Board_data,
	Update_entity_data,
	Entity_data,
	Player_movement_command,
	Player_movement_treated
};

#define SERVER_PORT 65000

using Connection = jgl::Connection<Server_message>;
using Message = jgl::Message<Server_message>;
using Server = jgl::Server<Server_message>;
using Client = jgl::Client<Server_message>;
using Server_manager = jgl::Server_manager<Server_message>;
#define SERVER_ACTIVITY(p_msg_type) Server_manager::instance()->add_activity(p_msg_type, [&](Connection* p_client, Message& p_msg, jgl::Data_contener& p_param)
using Client_manager = jgl::Client_manager<Server_message>;
#define CLIENT_ACTIVITY(p_msg_type) Client_manager::instance()->add_activity(p_msg_type, [&](Message& p_msg, jgl::Data_contener& p_param)
