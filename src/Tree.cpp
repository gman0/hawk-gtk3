#include <hawk/handlers/dir.h>
#include <exception>
#include <iostream>
#include "Tree.h"

using namespace std;
Tree::Tree(const hawk::List_dir* ld, Gtk::Box& box)
	:
	m_columns{new Model_columns},
	m_ref_tree_model{Gtk::ListStore::create(*m_columns)},
	m_tree_view{new Gtk::TreeView},
	m_scrolled_window{new Gtk::ScrolledWindow}
{
	// setup the tree view

	m_tree_view->set_model(m_ref_tree_model);
	m_tree_view->set_rules_hint();
	m_tree_view->append_column("Name", m_columns->entry);
	m_tree_view->set_headers_visible(false);

	// setup scrolling
	m_scrolled_window->add(*m_tree_view);
	m_scrolled_window->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	// add the TreeView
	box.pack_start(get_widget(), Gtk::PACK_EXPAND_WIDGET);

	// fill the tree

	const hawk::List_dir::Dir_cache* cache = ld->read();

	if (!cache)
		return;

	const hawk::List_dir::Dir_vector& vec = cache->vec;

	// insert the cache entries into the tree model
	Gtk::TreeModel::Row row;
	for (const auto& i : vec)
	{
		row = *(m_ref_tree_model->append());
		row[m_columns->entry] = i.filename().c_str();
	}

	// we need to have const_iterator's (because cache is const)
	hawk::List_dir::Dir_vector::const_iterator begin = vec.begin();
	hawk::List_dir::Dir_vector::const_iterator cursor = cache->cursor;

	// convert libhawk's cursor to tree's cursor
	auto tree_cursor = m_ref_tree_model->children().begin();
	for (int i = std::distance(begin, cursor) - 1; i >= 0; --i)
		++tree_cursor;

	// set the cursor
	Gtk::TreePath tpath {tree_cursor};
	m_tree_view->set_cursor(tpath);

	// register on_cursor_changed signal
	// Note that we're registering this signal AFTER setting the
	// cursor on purpose - we don't need to check the cursor for preview as
	// this was already done by libhawk when we created this tab.
	m_tree_view->signal_cursor_changed().connect(
		sigc::mem_fun(*this, &Tree::on_cursor_changed));

}

Gtk::Widget& Tree::get_widget()
{
	return static_cast<Gtk::Widget&>(*m_scrolled_window);
}

void Tree::on_cursor_changed()
{
	cout << "CURSOR CHANGED\n";
}
