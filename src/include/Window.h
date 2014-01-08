#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm.h>
#include <hawk/TabManager.h>
#include <hawk/TypeFactory.h>
#include <vector>
#include "Tree.h"

using Tree_vector = std::vector<Tree*>;

class Window : public Gtk::Window
{
private:
	hawk::Type_factory* m_type_factory;
	hawk::Tab_manager* m_tab_manager;

	hawk::Tab_manager::Tab_iterator m_current_tab;

	Tree_vector m_trees;

	Gtk::HBox m_tree_box;
	Gtk::HBox m_hbox_cd;
	Gtk::Box m_vbox;

	Gtk::ScrolledWindow m_scrolled_window;
	Gtk::ButtonBox m_button_box;
	Gtk::Button m_button_quit;
	Gtk::Label m_pwd_label;

	Gtk::Entry  m_entry_cd;
	Gtk::Button m_button_cd;

public:
	Window();
	virtual ~Window();

	inline Gtk::HBox& get_tree_box() { return m_tree_box; }
	inline Tree_vector& get_treevec() { return m_trees; }

	inline hawk::Tab_manager::Tab_iterator get_current_tab()
	{ return m_current_tab; }

	inline void redraw() { show_all_children(); }

protected:
	void on_button_quit();
	void on_button_cd();

};

#endif // WINDOW_H
