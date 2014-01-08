#include "DirPreview.h"
#include "tree_active.h"

Tree_active::~Tree_active()
{
	m_sig_cursor_change.disconnect();
}

Tree_active::Tree_active(const Tree_active& t)
	: Tree{t}
{
	m_sig_cursor_change =
		m_tree_view->signal_cursor_changed().connect(
			sigc::mem_fun(*this, &Tree_active::on_cursor_changed));
}

Tree_active::Tree_active(Tree_active&& t)
	: Tree{std::move(t)}
{
	m_sig_cursor_change =
		m_tree_view->signal_cursor_changed().connect(
			sigc::mem_fun(*this, &Tree_active::on_cursor_changed));
}

Tree_active& Tree_active::operator=(const Tree_active& t)
{
	Tree::operator=(t);
	
	m_sig_cursor_change =
		m_tree_view->signal_cursor_changed().connect(
			sigc::mem_fun(*this, &Tree_active::on_cursor_changed));
	
	return *this;
}

Tree_active& Tree_active::operator=(Tree_active&& t)
{
	Tree::operator=(std::move(t));

	m_sig_cursor_change =
		m_tree_view->signal_cursor_changed().connect(
			sigc::mem_fun(*this, &Tree_active::on_cursor_changed));
	
	return *this;
}

Tree_active::Tree_active(Dir_preview* dp, Gtk::Box& box)
	: Tree{dp, box}
{
	box.pack_start(get_widget(), Gtk::PACK_EXPAND_WIDGET);
	m_tree_view->set_sensitive();
	m_tree_view->grab_focus();

	m_separator->set_margin_left(2);
	m_separator->set_margin_right(2);
	box.pack_start(*m_separator, Gtk::PACK_SHRINK);

	// register on_cursor_changed signal
	// Note that we're registering this signal AFTER setting the
	// cursor on purpose - we don't need to check the cursor for preview as
	// this was already done by libhawk when we created this tab.
	m_sig_cursor_change =
		m_tree_view->signal_cursor_changed().connect(
			sigc::mem_fun(*this, &Tree_active::on_cursor_changed));
}

void Tree_active::update()
{
	// We need to block the on_cursor_changed signal
	// for the whole duration of updating the tree
	// so that we won't set the cursor recursively.
	m_sig_cursor_change.block();

	Tree::update();

	m_sig_cursor_change.unblock();
}

void Tree_active::on_cursor_changed()
{
	// get the position of the cursor

	if (m_ref_tree_model->children().size() == 0)
		return;

	Gtk::TreeModel::Path tree_path;
	Gtk::TreeViewColumn* tree_col;

	m_tree_view->get_cursor(tree_path, tree_col);

	int pos = std::distance(m_ref_tree_model->children().begin(),
						m_ref_tree_model->get_iter(tree_path));

	// set the cursor

	auto begin = m_handler->get_begin_cursor();
	m_handler->set_tab_cursor(begin + pos);

	m_handler->redraw();
}
