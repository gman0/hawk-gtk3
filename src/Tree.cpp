#include <iostream>
#include "Tree.h"
#include "DirPreview.h"

using namespace std;

Tree::Tree(const Tree& t)
	:
	m_handler{t.m_handler},
	m_columns{t.m_columns},
	m_ref_tree_model{t.m_ref_tree_model},
	m_tree_view{t.m_tree_view},
	m_separator{t.m_separator},
	m_scrolled_window{t.m_scrolled_window}
{}

Tree::Tree(Tree&& t)
	:
	m_handler{t.m_handler},
	m_columns{std::move(t.m_columns)},
	m_ref_tree_model{std::move(t.m_ref_tree_model)},
	m_tree_view{std::move(t.m_tree_view)},
	m_separator{std::move(t.m_separator)},
	m_scrolled_window{std::move(t.m_scrolled_window)}
{}

Tree& Tree::operator=(const Tree& t)
{
	m_handler = t.m_handler;
	m_columns = t.m_columns;
	m_ref_tree_model = t.m_ref_tree_model;
	m_tree_view = t.m_tree_view;
	m_separator = t.m_separator;
	m_scrolled_window = t.m_scrolled_window;

	return *this;
}

Tree& Tree::operator=(Tree&& t)
{
	m_handler = t.m_handler;
	m_columns = std::move(t.m_columns);
	m_ref_tree_model = std::move(t.m_ref_tree_model);
	m_separator = std::move(t.m_separator);
	m_tree_view = std::move(t.m_tree_view);
	m_scrolled_window = std::move(t.m_scrolled_window);

	return *this;
}

Tree::Tree(Dir_preview* dp, Gtk::Box& box)
	:
	m_handler{dp},
	m_columns{new Model_columns},
	m_ref_tree_model{Gtk::ListStore::create(*m_columns)},
	m_tree_view{new Gtk::TreeView},
	m_separator{new Gtk::VSeparator},
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

	// fill the tree

	const hawk::List_dir::Dir_cache* cache = dp->read();

	if (!cache)
		return;

	const hawk::List_dir::Dir_vector& vec = cache->vec;

	// insert the cache entries into the tree model,
	// but only if we have anything to insert

	if (vec.empty())
		return;

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
}

