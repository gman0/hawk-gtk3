#include <iostream>
#include <exception>
#include <hawk/handlers/dir.h>
#include <hawk/handlers/dir_hash.h>
#include "Window.h"
#include "DirPreview.h"

using namespace boost::filesystem;

constexpr unsigned ncols = 3;

Window::Window()
	:
	m_type_factory{new hawk::Type_factory},
	m_vbox{Gtk::ORIENTATION_VERTICAL},
	m_button_quit{"Quit"},
	m_button_cd{"cd"}
{
	// setup our window

	set_title("hawk-gtk3-test");
	set_border_width(5);
	set_default_size(600, 300);
	add(m_vbox);

	// setup a layout for our window

	m_vbox.pack_start(m_pwd_label, Gtk::PACK_SHRINK);
	m_vbox.pack_start(m_tree_box, Gtk::PACK_EXPAND_WIDGET);
	m_vbox.pack_start(m_hbox_cd, Gtk::PACK_SHRINK);
	m_vbox.pack_start(m_button_box, Gtk::PACK_SHRINK);
	m_button_box.pack_start(m_button_cd, Gtk::PACK_SHRINK);
	m_button_box.pack_start(m_button_quit, Gtk::PACK_SHRINK);
	m_button_box.set_border_width(5);
	m_button_box.set_layout(Gtk::BUTTONBOX_END);

	m_hbox_cd.set_border_width(5);
	m_hbox_cd.pack_start(m_entry_cd, Gtk::PACK_EXPAND_WIDGET);

	// connect the button to a callback function
	m_button_quit.signal_clicked().connect(sigc::mem_fun(*this, &Window::on_button_quit));
	m_button_cd.signal_clicked().connect(sigc::mem_fun(*this, &Window::on_button_cd));

	// register List_dir handler

	hawk::Type_factory::Type_product tp =
		[this](const path& p, hawk::Column* parent_column)
			{ return new Dir_preview{p, parent_column, this, ncols}; };

	m_type_factory->register_type(
		hawk::get_handler_hash<hawk::List_dir>(), tp);

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

void Window::on_button_quit()
{
	hide();
}

void Window::on_button_cd()
{
	boost::filesystem::path pwd { m_entry_cd.get_text().c_str() };
	m_current_tab->set_pwd(pwd);

	m_pwd_label.set_text(pwd.c_str());

	for (Tree* tree : m_trees)
	{
		if (tree)
			tree->update();
	}

	redraw();
}
