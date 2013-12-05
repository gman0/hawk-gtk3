#include <hawk/handlers/dir.h>
#include <exception>
#include <iostream>
#include "Tree.h"

using namespace std;

Tree::~Tree()
{
	m_sig_cursor_change.disconnect();
}

Tree::Tree(const Tree& t)
	:
	m_columns{t.m_columns},
	m_ref_tree_model{t.m_ref_tree_model},
	m_tree_view{t.m_tree_view},
	m_separator{t.m_separator},
	m_scrolled_window{t.m_scrolled_window},
	m_flags{t.m_flags}
{
	m_sig_cursor_change =
		m_tree_view->signal_cursor_changed().connect(
			sigc::mem_fun(*this, &Tree::on_cursor_changed));
}

Tree::Tree(Tree&& t)
	:
	m_columns{std::move(t.m_columns)},
	m_ref_tree_model{std::move(t.m_ref_tree_model)},
	m_tree_view{std::move(t.m_tree_view)},
	m_separator{std::move(t.m_separator)},
	m_scrolled_window{std::move(t.m_scrolled_window)},
	m_flags{t.m_flags}
{
	m_sig_cursor_change =
		m_tree_view->signal_cursor_changed().connect(
			sigc::mem_fun(*this, &Tree::on_cursor_changed));
}

Tree& Tree::operator=(const Tree& t)
{
	m_columns = t.m_columns;
	m_ref_tree_model = t.m_ref_tree_model;
	m_tree_view = t.m_tree_view;
	m_separator = t.m_separator;
	m_scrolled_window = t.m_scrolled_window;
	m_flags = t.m_flags;

	m_sig_cursor_change =
		m_tree_view->signal_cursor_changed().connect(
			sigc::mem_fun(*this, &Tree::on_cursor_changed));

	return *this;
}

Tree& Tree::operator=(Tree&& t)
{
	m_columns = std::move(t.m_columns);
	m_ref_tree_model = std::move(t.m_ref_tree_model);
	m_separator = std::move(t.m_separator);
	m_tree_view = std::move(t.m_tree_view);
	m_scrolled_window = std::move(t.m_scrolled_window);
	m_flags = t.m_flags;

	m_sig_cursor_change =
		m_tree_view->signal_cursor_changed().connect(
			sigc::mem_fun(*this, &Tree::on_cursor_changed));

	return *this;
}

Tree::Tree(const hawk::List_dir* ld, Gtk::Box& box, Tree::Tree_flags flags)
	:
	m_columns{new Model_columns},
	m_ref_tree_model{Gtk::ListStore::create(*m_columns)},
	m_tree_view{new Gtk::TreeView},
	m_separator{new Gtk::VSeparator},
	m_scrolled_window{new Gtk::ScrolledWindow},
	m_flags{flags}
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
	if (flags == TF_SHRINK)
	{
		m_tree_view->set_can_focus(false);
		box.pack_start(get_widget(), Gtk::PACK_SHRINK);
		m_scrolled_window->set_size_request(40);
	}
	else if (flags & TF_EXPAND)
	{
		box.pack_start(get_widget(), Gtk::PACK_EXPAND_WIDGET);

		if ((flags & TF_ACTIVE) == TF_ACTIVE)
			m_tree_view->grab_focus();
	}

	// add the separator
	m_separator->set_margin_left(2);
	m_separator->set_margin_right(2);
	box.pack_start(*m_separator, Gtk::PACK_SHRINK);

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
	m_sig_cursor_change =
		m_tree_view->signal_cursor_changed().connect(
			sigc::mem_fun(*this, &Tree::on_cursor_changed));
}

Gtk::Widget& Tree::get_widget()
{
	return static_cast<Gtk::Widget&>(*m_scrolled_window);
}

void Tree::on_cursor_changed()
{
	cout << "CURSOR CHANGED ";
	if (m_flags != TF_ACTIVE) return;
}
