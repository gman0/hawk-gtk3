/*
	Copyright (C) 2013-2014 Róbert "gman" Vašek <gman@codefreax.org>

	This file is part of hawk-gtk3.

	hawk-gtk3 is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	hawk-gtk3 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with hawk-gtk3.  If not, see <http://www.gnu.org/licenses/>.
*/

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

	// commands
	void set_pwd(const boost::filesystem::path& pwd);
	void rename(const boost::filesystem::path& new_p);
	void copy(const boost::filesystem::path& to);
	void move(const boost::filesystem::path& to);
	void remove();
	void remove_all();
	void mkdir(const boost::filesystem::path& p);
	void symlink(const boost::filesystem::path& new_symlink);
	void hardlink(const boost::filesystem::path& new_hard_link);

	void set_infobar_msg(const std::string& msg);

private:
	void register_handlers();
	void register_commands();

	void on_button_quit();
	void on_button_cmd();
	void on_infobar_response(int response);

	// refreshes the current working path by setting it to the current directory
	inline void refresh() { set_pwd("."); }

	// check error_code and eventually refresh
	void check_error(const boost::system::error_code& ec);

	// scrolls up by one position after calling remove/remove_all
	void scroll_up();
};

#endif // WINDOW_H
