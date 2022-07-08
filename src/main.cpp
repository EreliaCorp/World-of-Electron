#include "woe.h"

int main()
{
	jgl::Application app("World of Electron", jgl::Vector2Int(840, 680), jgl::Color(50, 50, 50));
	app.set_default_font("ressource/font/karma suture.ttf");
	app.active_multithread();

	Main_application* main_app = new Main_application(nullptr);
	main_app->set_geometry(0, app.size());
	main_app->activate();

	return (app.run());
}