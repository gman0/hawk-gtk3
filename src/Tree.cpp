#include <iostream>
#include "Tree.h"
#include "DirPreview.h"

using namespace std;

Tree::Tree(Dir_preview* dp, Gtk::Box& box)
	:
	m_handler{dp},
	m_columns{new Model_columns},
	m_ref_tree_model{Gtk::ListStore::create(*m_columns)},
	m_tree_view{new Gtk::TreeView},
	m_separator{new Gtk::VSeparator},
	m_scrolled_window{new Gtk::ScrolledWindow},
	m_empty{new Gtk::Label{"empty"}}
{
	// setup the tree view

	m_tree_view->set_model(m_ref_tree_model);
	m_tree_view->set_rules_hint();
	m_tree_view->append_column("Name", m_columns->entry);
	m_tree_view->set_headers_visible(false);

	// setup scrolling
	m_scrolled_window->add(*m_tree_view);
	m_scrolled_window->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	// TODO: this
	// m_vbox_empty->pack_start(*m_empty, Gtk::PACK_EXPAND_WIDGET);
	// box.pack_start(*m_empty, Gtk::PACK_EXPAND_WIDGET);

	update();
}

void Tree::update()
{
	// insert the dir entries into the tree model,
	// but only if we have anything to insert

	if (!m_ref_tree_model->children().empty())
		m_ref_tree_model->clear();

	const hawk::List_dir::Dir_vector& vec = m_handler->get_contents();

	if (vec.empty())
	{
		m_empty->show();
		return;
	}

	// m_empty->hide();

	Gtk::TreeModel::Row row;
	for (const auto& i : vec)
	{
		row = *(m_ref_tree_model->append());
		row[m_columns->entry] = i.filename().c_str();
	}

	// we need to have const_iterator's (because cache is const)
	hawk::List_dir::Dir_vector::const_iterator begin = vec.begin();
	hawk::List_dir::Dir_vector::const_iterator cursor = m_handler->get_cursor();

	// convert libhawk's cursor to tree's cursor
	auto tree_cursor = m_ref_tree_model->children().begin();
	for (int i = std::distance(begin, cursor) - 1; i >= 0; --i)
		++tree_cursor;

	// set the cursor
	Gtk::TreePath tpath {tree_cursor};
	m_tree_view->set_cursor(tpath);
}
