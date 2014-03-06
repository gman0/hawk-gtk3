#include <iostream>
#include <exception>
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

using namespace boost::filesystem;
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

	set_title("hawk-gtk3-test");
	set_border_width(5);
	set_default_size(600, 300);
	add(m_vbox);

	// setup a layout for our window

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

	// connect the button to a callback function
	m_button_quit.signal_clicked().connect(sigc::mem_fun(*this, &Window::on_button_quit));
	m_button_cmd.signal_clicked().connect(sigc::mem_fun(*this, &Window::on_button_cmd));

	Gtk::Container* infobar_container =
		dynamic_cast<Gtk::Container*>(m_infobar.get_content_area());

	if (infobar_container)
		infobar_container->add(m_infobar_msg);

	m_infobar_msg.show();

	m_infobar.set_no_show_all();
	m_infobar.add_button("Ok", 0);
	m_bottom.pack_start(m_infobar, Gtk::PACK_SHRINK);

	m_infobar.signal_response().connect(sigc::mem_fun(*this, &Window::on_infobar_response));

	register_commands();

	// *-*-* hawk *-*-*
	
	register_handlers();

	// only after the List_dir handler is registered we can instantiate Tab_manager
	m_tab_manager = new hawk::Tab_manager {m_type_factory, ncols};

	// create a new tab
	m_current_tab = m_tab_manager->add_tab("/home/gman");

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
		[this](const string& arg){ set_pwd(path{arg}); return true; });

	// rename command
	m_cmd.register_command("rename",
		[this](const string& arg){ rename(path{arg}); return true; });
}

void Window::on_button_quit()
{
	hide();
}

void Window::on_button_cmd()
{
	m_cmd(m_entry_cmd.get_text());
}

void Window::on_infobar_response(int response)
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

void Window::set_pwd(const boost::filesystem::path& pwd)
{
	boost::system::error_code ec;
	m_current_tab->set_pwd(pwd, ec);

	if (ec)
	{
		set_infobar_msg(ec.message());
		return;
	}

	update_and_redraw();
}

void Window::set_infobar_msg(const std::string& msg)
{
	m_infobar_msg.set_text(msg);
	m_infobar.set_message_type(Gtk::MESSAGE_INFO);
	m_infobar.show();
}

void Window::rename(const path& new_p)
{
	boost::system::error_code ec;

	const hawk::List_dir* ld = dynamic_cast<const hawk::List_dir*>(
		m_current_tab->get_active_column()->get_handler());
	path old_p { ld->get_cursor()->path };

	hawk::rename(old_p, m_current_tab->get_pwd() / new_p, &(*m_current_tab), ec);

	if (ec)
	{
		set_infobar_msg(ec.message());
		return;
	}

	set_pwd(".");
}
