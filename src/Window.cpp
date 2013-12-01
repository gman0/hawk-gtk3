#include <iostream>
#include <exception>
#include <hawk/handlers/dir.h>
#include <hawk/handlers/dir_hash.h>
#include "Window.h"
#include "DirPreview.h"

using namespace boost::filesystem;

Window::Window()
	:
	m_type_factory{new hawk::Type_factory},
	m_tab_manager{new hawk::Tab_manager{m_type_factory}},
	m_vbox{Gtk::ORIENTATION_VERTICAL},
	m_button_quit{"Quit"}
{
	set_title("hawk-gtk3-test");
	set_border_width(5);
	set_default_size(600, 300);
	add(m_vbox);

	m_vbox.pack_start(m_pwd_label, Gtk::PACK_SHRINK);
	m_vbox.pack_start(m_tree_box, Gtk::PACK_EXPAND_WIDGET);
	m_vbox.pack_start(m_button_box, Gtk::PACK_SHRINK);
	m_button_box.pack_start(m_button_quit, Gtk::PACK_SHRINK);
	m_button_box.set_border_width(5);
	m_button_box.set_layout(Gtk::BUTTONBOX_END);

	m_button_quit.signal_clicked().connect(sigc::mem_fun(*this, &Window::on_button_quit));

	hawk::Type_factory::Type_product tp =
		[this](const path& p)
			{ return new Dir_preview{m_tree_box, &m_trees, p}; };

	m_type_factory->register_type(
		hawk::get_handler_hash<hawk::List_dir>(), tp);

	hawk::Tab_manager::Tab_iterator tab = m_tab_manager->add_tab("/home/gman");
	m_pwd_label.set_text(tab->get_pwd().c_str());

	show_all_children();
}

Window::~Window()
{
	delete m_tab_manager;
	delete m_type_factory;
}

void Window::on_button_quit()
{
	hide();
}
