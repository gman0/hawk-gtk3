#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm.h>
#include <hawk/TabManager.h>
#include <hawk/TypeFactory.h>
#include <vector>
#include "Tree.h"

class Window : public Gtk::Window
{
private:
	hawk::Type_factory* m_type_factory;
	hawk::Tab_manager* m_tab_manager;

	hawk::Tab_manager::Tab_iterator m_current_tab;

	std::vector<Tree> m_trees;

	Gtk::HBox m_tree_box;
	Gtk::Box m_vbox;

	Gtk::ScrolledWindow m_scrolled_window;
	Gtk::ButtonBox m_button_box;
	Gtk::Button m_button_quit;
	Gtk::Label m_pwd_label;

public:
	Window();
	virtual ~Window();

	Gtk::HBox& get_tree_box() { return m_tree_box; }
	std::vector<Tree>& get_treevec() { return m_trees; }
	hawk::Tab_manager::Tab_iterator get_current_tab();

protected:
	void on_button_quit();

};

#endif // WINDOW_H
