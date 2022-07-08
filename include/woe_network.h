#pragma once

#include "jgl.h"

enum class Server_message
{
	Request_player_information,
	Player_information,
};

#define SERVER_PORT 65000

using Connection = jgl::Connection<Server_message>;
using Message = jgl::Message<Server_message>;
using Server = jgl::Server<Server_message>;
using Client = jgl::Client<Server_message>;
using Server_manager = jgl::Server_manager<Server_message>;
using Client_manager = jgl::Client_manager<Server_message>;
