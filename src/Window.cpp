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

// This is needed because of undefined reference to `boost::filesystem::detail::copy_file
#define BOOST_NO_CXX11_SCOPED_ENUMS

#include <hawk/handlers/List_dir.h>
#include <hawk/handlers/List_dir_hash.h>
#include <hawk/fsctl.h>
#include <boost/system/error_code.hpp>
#include "Window.h"
#include "previews/Dir_preview.h"
#include "previews/Image_preview.h"
#include "previews/Image_preview_hash_extern.h"
#include "previews/Text_preview.h"
#include "previews/Text_preview_hash_extern.h"
#include "previews/Tree_active.h"

using namespace boost::filesystem;
using boost::system::error_code;
using namespace std;

constexpr unsigned ncols = 3;

Window::Window()
	:
	m_type_factory{new hawk::Type_factory},
	m_vbox{Gtk::ORIENTATION_VERTICAL},
	m_button_quit{"_Quit", true},
	m_button_cmd{"_cmd", true}
{
	// setup our window

	set_title("hawk-gtk3");
	set_border_width(5);
	set_default_size(600, 300);
	add(m_vbox);

	// setup a layout for the window

	m_vbox.pack_start(m_pwd_label, Gtk::PACK_SHRINK);
	m_vbox.pack_start(m_tree_box, Gtk::PACK_EXPAND_WIDGET);
	m_vbox.pack_start(m_hbox_cmd, Gtk::PACK_SHRINK);

	m_vbox.pack_start(m_bottom, Gtk::PACK_SHRINK);
	m_bottom.pack_end(m_button_box, Gtk::PACK_SHRINK);

	// m_vbox.pack_start(m_button_box, Gtk::PACK_SHRINK);
	m_button_box.pack_start(m_button_cmd, Gtk::PACK_SHRINK);
	m_button_box.pack_start(m_button_quit, Gtk::PACK_SHRINK);
	m_button_box.set_border_width(5);
	m_button_box.set_layout(Gtk::BUTTONBOX_END);

	m_hbox_cmd.set_border_width(5);
	m_hbox_cmd.pack_start(m_entry_cmd, Gtk::PACK_EXPAND_WIDGET);

	// connect the buttons to ther respective callback function
	m_button_quit.signal_clicked().
		connect(sigc::mem_fun(*this, &Window::on_button_quit));
	m_button_cmd.signal_clicked().
		connect(sigc::mem_fun(*this, &Window::on_button_cmd));

	Gtk::Container* infobar_container =
		dynamic_cast<Gtk::Container*>(m_infobar.get_content_area());

	if (infobar_container)
		infobar_container->add(m_infobar_msg);

	m_infobar_msg.show();

	m_infobar.set_no_show_all();
	m_infobar.add_button("Ok", 0);
	m_bottom.pack_start(m_infobar, Gtk::PACK_SHRINK);

	m_infobar.signal_response().
		connect(sigc::mem_fun(*this, &Window::on_infobar_response));

	register_commands();

	// *-*-* hawk *-*-*
	
	register_handlers();

	m_trees.reserve(ncols + 1);
	m_trees.shrink_to_fit();

	// only after the List_dir handler is registered we can instantiate Tab_manager
	m_tab_manager = new hawk::Tab_manager {m_type_factory, ncols};

	// create a new tab
	m_current_tab = m_tab_manager->add_tab(current_path());

	// print our working directory at the top of the window
	m_pwd_label.set_text(m_current_tab->get_pwd().c_str());

	// we're done
	show_all_children();
}

Window::~Window()
{
	delete m_tab_manager;
	delete m_type_factory;

	for (Tree* tree : m_trees)
		delete tree;
}

void Window::register_handlers()
{
	// register List_dir handler

	hawk::Type_factory::Type_product list_dir_handler =
		[this](const path& p, hawk::Column* parent_column)
			{ return new Dir_preview{p, parent_column, this, ncols}; };

	m_type_factory->register_type(
		hawk::get_handler_hash<hawk::List_dir>(), list_dir_handler);

	// register Image_preview handler
	
	hawk::Type_factory::Type_product image_preview_handler =
		[this](const path& p, hawk::Column* parent_column)
			{ return new Image_preview{p, parent_column, this}; };

	m_type_factory->register_type(
		hawk::get_handler_hash<Image_preview>(), image_preview_handler);
	
	// register Text_preview handler
	
	hawk::Type_factory::Type_product text_preview_handler =
		[this](const path& p, hawk::Column* parent_column)
			{ return new Text_preview{p, parent_column, this}; };

	m_type_factory->register_type(
		hawk::get_handler_hash<Text_preview>(), text_preview_handler);
}

void Window::register_commands()
{
	// cd command
	m_cmd.register_command("cd",
		[this](const string& arg){ set_pwd(arg); return true; });

	// rename command
	m_cmd.register_command("rename",
		[this](const string& arg){ rename(arg); return true; });

	// copy
	m_cmd.register_command("cp",
		[this](const string& arg){ copy(arg); return true; });

	//move
	m_cmd.register_command("mv",
		[this](const string& arg){ move(arg); return true; });

	// remove command
	m_cmd.register_command("delete",
		[this](const string&){ remove(); return true; });
	m_cmd.register_command("delete_all",
		[this](const string&){ remove_all(); return true; });

	// mkdir command
	m_cmd.register_command("mkdir",
		[this](const string& arg){ mkdir(arg); return true; });

	// symlink command
	m_cmd.register_command("symlink",
		[this](const string& arg){ symlink(arg); return true; });

	// hardlink command
	m_cmd.register_command("hardlink",
		[this](const string& arg){ hardlink(arg); return true; });
}

void Window::on_button_quit()
{
	hide();
}

void Window::on_button_cmd()
{
	m_cmd(m_entry_cmd.get_text());
}

void Window::on_infobar_response(int)
{
	m_infobar_msg.set_text("");
	m_infobar.hide();
}

void Window::update_and_redraw()
{
	m_pwd_label.set_text(m_current_tab->get_pwd().c_str());

	for (Tree* tree : m_trees)
	{
		if (tree)
			tree->update();
	}

	redraw();
}

void Window::set_pwd(const path& pwd)
{
	error_code ec;
	m_current_tab->set_pwd(pwd, ec);

	if (ec)
	{
		set_infobar_msg(ec.message());
		return;
	}

	update_and_redraw();
}

void Window::set_infobar_msg(const string& msg)
{
	m_infobar_msg.set_text(msg);
	m_infobar.set_message_type(Gtk::MESSAGE_INFO);
	m_infobar.show();
}

void Window::check_error(const error_code& ec)
{
	if (ec)
	{
		set_infobar_msg(ec.message());
		return;
	}

	refresh();
}

void Window::scroll_up()
{
	// find out which Tree is Tree_active:
	//it's either the last one
	Tree_active* tree = dynamic_cast<Tree_active*>(m_trees.back());
	if (tree == nullptr)
	{
		// or the last but one
		tree = static_cast<Tree_active*>(*(m_trees.end() - 2));
	}

	auto children = tree->get_tree_model()->children();
	if (children.size() > 1)
	{
		auto cursor = tree->get_cursor();

		// if the removed file/directory was at the top
		if (children.begin() == cursor)
			++cursor; // scroll down
		else
			--cursor; // otherwise scroll up

		int id = cursor->get_value(tree->get_columns()->id);

		m_current_tab->set_cursor(
			m_current_tab->get_begin_cursor() + id);

		redraw();
	}
}

void Window::rename(const path& new_p)
{
	boost::system::error_code ec;
	const hawk::List_dir* ld = m_current_tab->get_active_ld();
	path old_p { ld->get_cursor()->path };

	hawk::rename(old_p, m_current_tab->get_pwd() / new_p, &(*m_current_tab), ec);
	check_error(ec);
}

void Window::copy(const path& to)
{
	boost::system::error_code ec;
	const hawk::List_dir* ld = m_current_tab->get_active_ld();

	hawk::copy(ld->get_cursor()->path, canonical(to,
		m_current_tab->get_pwd()), ec);
	check_error(ec);
}

void Window::move(const path& to)
{
	boost::system::error_code ec;
	const hawk::List_dir* ld = m_current_tab->get_active_ld();

	hawk::move(ld->get_cursor()->path, canonical(to,
		m_current_tab->get_pwd()), ec);
	scroll_up();
	check_error(ec);
}



void Window::remove()
{
	error_code ec;
	const hawk::List_dir* ld = m_current_tab->get_active_ld();

	hawk::remove(ld->get_cursor()->path, ec);
	scroll_up();
	check_error(ec);
}

void Window::remove_all()
{
	error_code ec;
	const hawk::List_dir* ld = m_current_tab->get_active_ld();

	hawk::remove_all(ld->get_cursor()->path, ec);
	scroll_up();
	check_error(ec);
}

void Window::mkdir(const path& p)
{
	error_code ec;
	hawk::create_directories(p, ec);
	check_error(ec);
}

void Window::symlink(const path& new_symlink)
{
	error_code ec;
	const hawk::List_dir* ld = m_current_tab->get_active_ld();

	hawk::create_symlink(ld->get_cursor()->path, new_symlink, ec);
	check_error(ec);
}

void Window::hardlink(const path& new_hard_link)
{
	error_code ec;
	const hawk::List_dir* ld = m_current_tab->get_active_ld();

	hawk::create_hard_link(ld->get_cursor()->path, new_hard_link, ec);
	check_error(ec);
}
