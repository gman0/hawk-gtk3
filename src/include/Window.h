#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm.h>
#include <hawk/TabManager.h>
#include <hawk/TypeFactory.h>
#include <vector>
#include <boost/filesystem/path.hpp>
#include "previews/Tree.h"

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
	Gtk::HBox m_bottom;
	Gtk::Box  m_vbox;

	Gtk::ScrolledWindow m_scrolled_window;

	Gtk::ButtonBox m_button_box;
	Gtk::Button m_button_quit;
	Gtk::Label m_pwd_label;

	Gtk::Entry  m_entry_cd;
	Gtk::Button m_button_cd;

	Gtk::InfoBar m_infobar;
	Gtk::Label   m_infobar_msg;

public:
	Window();
	virtual ~Window();

	inline Gtk::HBox& get_tree_box() { return m_tree_box; }
	inline Tree_vector& get_treevec() { return m_trees; }

	inline hawk::Tab_manager::Tab_iterator get_current_tab()
	{ return m_current_tab; }

	inline void redraw() { show_all_children(); }
	void update_and_redraw();

	void set_pwd(const boost::filesystem::path& pwd);

	void set_infobar_msg(const std::string& msg);

protected:
	void on_button_quit();
	void on_button_cd();
	void on_infobar_response(int response);

};

#endif // WINDOW_H
