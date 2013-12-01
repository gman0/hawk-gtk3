#include <gtkmm/main.h>
#include "Window.h"

int main(int argc, char* argv[])
{
	Gtk::Main kit(argc, argv);

	Window win;
	Gtk::Main::run(win);

	return 0;
}
