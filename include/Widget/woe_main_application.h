#pragma once

#include "jgl.h"

#include "structure/woe_engine.h"

class Main_application : public jgl::Widget
{
public:
	struct Configuration_file
	{
		jgl::String server_address = "localhost";
		jgl::Int server_port = 6500;
		jgl::Bool server_mode = false;

		void encode(jgl::String p_file_path)
		{
			jgl::File file = jgl::open_file(p_file_path, jgl::File_mode::out);

			file << "Address:" << server_address << "\n";
			file << jgl::itoa(server_port) << "\n";
			file << "Server mode:" << std::boolalpha << server_mode << "\n";

			file.close();
		}
		void decode(jgl::String p_file_path)
		{
			jgl::File file = jgl::open_file(p_file_path, jgl::File_mode::in);

			server_address = jgl::get_strsplit(file, ";", 2)[1];
			server_port = jgl::stoi(jgl::get_strsplit(file, ";", 2)[1]);
			server_mode = ((jgl::get_strsplit(file, ";", 2)[1] == "true" ? true : false));

			file.close();
		}
	};
	struct Context
	{
		Configuration_file config;
		jgl::Long id = -1;
		Engine engine;
	};
	enum class Event
	{

	};
	enum class State
	{
		Init,
		In_game
	};
	using Publisher = jgl::Singleton<jgl::Publisher<Event, Context>>;
	using State_machine = jgl::Singleton<jgl::State_machine<State>>;
	
	class Init_activity : public jgl::State_machine<State>::Abstract_activity
	{
	private:

		enum class Internal_state
		{
			Waiting_client_connection,
			Asking_player_information,
			Waiting_player_information
		};

		Internal_state _state;
		jgl::Timer _player_information_timer;

	public:
		Init_activity(jgl::State_machine<State>* p_owner) :
			Abstract_activity(p_owner)
		{

		}
		void execute();
		void on_transition();
	};

private:
	Context* _context;

	void _render();
	void _on_geometry_change();

	jgl::Bool _update();
	jgl::Bool _fixed_update();

	void _initialize_network();

public:
	Main_application(jgl::Widget* p_parent);
};

extern Main_application::Context* g_context;