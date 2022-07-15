#pragma once

#include "jgl.h"

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