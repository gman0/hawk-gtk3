#ifndef TREE_H
#define TREE_H

#include <utility>
#include <memory>
#include <hawk/Column.h>
#include <gtkmm.h>

class Tree
{
public:
	enum Tree_flags
	{
		TF_SHRINK = 0, // TF_SHRINK won't grab focus
		TF_EXPAND = 1 << 1, // TF_EXPAND will expand but won't grab focus
		TF_ACTIVE = 1 << 2 | TF_EXPAND // TF_ACTIVE will both expand and grab focus
	};

private:
	struct Model_columns : public Gtk::TreeModel::ColumnRecord
	{
		Gtk::TreeModelColumn<Glib::ustring> entry;

		Model_columns()
		{
			add(entry);
		}
	};

	// With all the moving/copying around, we need to store the result
	// of sigc's connect() function in order to properly disconnect it
	// during Tree's destruction and then reconnect it again after copying.
	sigc::connection m_sig_cursor_change;

	// we have to use pointers (shared_ptr's) because copying all
	// of these member variables is forbidden
	std::shared_ptr<Model_columns> m_columns;

	Glib::RefPtr<Gtk::ListStore> m_ref_tree_model;
	std::shared_ptr<Gtk::TreeView> m_tree_view;

	std::shared_ptr<Gtk::VSeparator> m_separator;
	std::shared_ptr<Gtk::ScrolledWindow> m_scrolled_window;

	Tree_flags m_flags;

public:
	~Tree();
	Tree(const Tree& t);
	Tree(Tree&& t);
	Tree& operator=(const Tree& t);
	Tree& operator=(Tree&& t);

	Tree(const hawk::List_dir* ld, Gtk::Box& box, Tree_flags flags);

	Gtk::Widget& get_widget();

private:
	void on_cursor_changed();
};

#endif // TREE_H
