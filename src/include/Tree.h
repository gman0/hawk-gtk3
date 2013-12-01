#ifndef TREE_H
#define TREE_H

#include <utility>
#include <memory>
#include <hawk/Column.h>
#include <gtkmm.h>

class Tree
{
private:
	struct Model_columns : public Gtk::TreeModel::ColumnRecord
	{
		Gtk::TreeModelColumn<Glib::ustring> entry;

		Model_columns()
		{
			add(entry);
		}
	};
	
	// we have to use pointers because copying all of these
	// member variables is forbidden

	std::shared_ptr<Model_columns> m_columns;

	Glib::RefPtr<Gtk::ListStore> m_ref_tree_model;
	std::shared_ptr<Gtk::TreeView> m_tree_view;

	std::shared_ptr<Gtk::ScrolledWindow> m_scrolled_window;

public:
	Tree(const hawk::List_dir* ld, Gtk::Box& box);

	Gtk::Widget& get_widget();

private:
	void on_cursor_changed();
};

#endif // TREE_H
