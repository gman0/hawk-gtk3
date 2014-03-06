#ifndef WINDOW_H
#define WINDOW_H

#include <gtkmm.h>
#include <hawk/Tab_manager.h>
#include <hawk/Type_factory.h>
#include <vector>
#include <boost/filesystem/path.hpp>
#include "Cmd.h"
#include "previews/Tree.h"

using Tree_vector = std::vector<Tree*>;

class Window : public Gtk::Window
{
private:
	hawk::Type_factory* m_type_factory;
	hawk::Tab_manager* m_tab_manager;

	hawk::Tab_manager::Tab_iterator m_current_tab;

	Cmd m_cmd;

	Tree_vector m_trees;

	Gtk::HBox m_tree_box;
	Gtk::HBox m_hbox_cmd;
	Gtk::HBox m_bottom;
	Gtk::Box  m_vbox;

	Gtk::ScrolledWindow m_scrolled_window;

	Gtk::ButtonBox m_button_box;
	Gtk::Button m_button_quit;
	Gtk::Label m_pwd_label;

	Gtk::Entry  m_entry_cmd;
	Gtk::Button m_button_cmd;

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
	void rename(const boost::filesystem::path& new_p);

	void set_infobar_msg(const std::string& msg);

protected:
	void register_handlers();
	void register_commands();

	void on_button_quit();
	void on_button_cmd();
	void on_infobar_response(int response);

};

#endif // WINDOW_H
