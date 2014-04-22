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

#ifndef TREE_H
#define TREE_H

#include <utility>
#include <memory>
#include <hawk/Column.h>

#include <gtkmm/treemodel.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/hvseparator.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>

class Dir_preview;
class Tree
{
protected:
	struct Model_columns : public Gtk::TreeModel::ColumnRecord
	{
		Gtk::TreeModelColumn<Glib::ustring> entry;
		Gtk::TreeModelColumn<int> id;

		Model_columns()
		{
			add(entry);
			add(id);
		}
	};

	Dir_preview* m_handler;

	// we have to use pointers (shared_ptr's) because copying all
	// of these member variables is forbidden
	std::shared_ptr<Model_columns> m_columns;

	Glib::RefPtr<Gtk::ListStore> m_tree_model;
	std::shared_ptr<Gtk::TreeView> m_tree_view;

	std::shared_ptr<Gtk::VSeparator> m_separator;
	std::shared_ptr<Gtk::ScrolledWindow> m_scrolled_window;

	std::shared_ptr<Gtk::Label> m_empty;

public:
	virtual ~Tree() = default;

	Tree(Dir_preview* dp, Gtk::Box& box);

	inline Gtk::Widget& get_widget()
	{
		return static_cast<Gtk::Widget&>(*m_scrolled_window);
	}

	virtual void update();

	inline bool empty() { return m_tree_model->children().empty(); }
	Glib::RefPtr<Gtk::ListStore> get_tree_model() { return m_tree_model; }
	std::shared_ptr<Model_columns> get_columns() { return m_columns; }
	Gtk::TreeModel::iterator get_cursor();

private:
	Gtk::TreeModel::iterator get_tree_iter(int id,
		const Gtk::TreeModel::Children& children);
};

#endif // TREE_H
