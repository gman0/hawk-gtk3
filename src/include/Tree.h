#ifndef TREE_H
#define TREE_H

#include <utility>
#include <memory>
#include <hawk/Column.h>
#include <gtkmm.h>

class Dir_preview;
class Tree
{
protected:
	struct Model_columns : public Gtk::TreeModel::ColumnRecord
	{
		Gtk::TreeModelColumn<Glib::ustring> entry;

		Model_columns()
		{
			add(entry);
		}
	};

	Dir_preview* m_handler;

	// we have to use pointers (shared_ptr's) because copying all
	// of these member variables is forbidden
	std::shared_ptr<Model_columns> m_columns;

	Glib::RefPtr<Gtk::ListStore> m_ref_tree_model;
	std::shared_ptr<Gtk::TreeView> m_tree_view;

	std::shared_ptr<Gtk::VSeparator> m_separator;
	std::shared_ptr<Gtk::ScrolledWindow> m_scrolled_window;

public:
	virtual ~Tree() {}
	Tree(const Tree& t);
	Tree(Tree&& t);
	Tree& operator=(const Tree& t);
	Tree& operator=(Tree&& t);

	Tree(Dir_preview* dp, Gtk::Box& box);

	inline Gtk::Widget& get_widget()
	{
		return static_cast<Gtk::Widget&>(*m_scrolled_window);
	}
};

#endif // TREE_H
