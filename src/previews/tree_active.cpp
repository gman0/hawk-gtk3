#include <gtk-3.0/gdk/gdkkeysyms.h>
#include <gdk/gdk.h>
#include <hawk/TabManager.h>
#include "Window.h"
#include "previews/DirPreview.h"
#include "previews/tree_active.h"

#include <iostream>
using namespace std;
Tree_active::~Tree_active()
{
	m_sig_cursor_change.disconnect();
	m_sig_kb_press.disconnect();
	m_sig_kb_release.disconnect();
}

Tree_active::Tree_active(const Tree_active& t)
	: Tree{t}
{
	register_signals();
}

Tree_active::Tree_active(Tree_active&& t)
	: Tree{std::move(t)}
{
	register_signals();
}

Tree_active& Tree_active::operator=(const Tree_active& t)
{
	Tree::operator=(t);

	register_signals();

	return *this;
}

Tree_active& Tree_active::operator=(Tree_active&& t)
{
	Tree::operator=(std::move(t));

	register_signals();

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

	// register the signals
	// Note that we're registering on_cursor_change signal AFTER setting the
	// cursor on purpose - we don't need to check the cursor for preview as
	// this was already done by libhawk when we created this tab.

	register_signals();
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

void Tree_active::register_signals()
{
	m_sig_cursor_change =
		m_tree_view->signal_cursor_changed().connect(
			sigc::mem_fun(*this, &Tree_active::on_cursor_changed));
	m_sig_kb_press =
		m_tree_view->signal_key_press_event().connect(
			sigc::mem_fun(*this, &Tree_active::on_kb_press));
	m_sig_kb_release =
		m_tree_view->signal_key_release_event().connect(
			sigc::mem_fun(*this, &Tree_active::on_kb_press));


	m_tree_view->add_events(Gdk::KEY_PRESS_MASK | Gdk::KEY_RELEASE_MASK);
}

void Tree_active::on_cursor_changed()
{
	// get the position of the cursor

	if (m_tree_model->children().size() == 0)
		return;

	Gtk::TreeModel::Path tree_path;
	Gtk::TreeViewColumn* tree_col;

	m_tree_view->get_cursor(tree_path, tree_col);

	int id = m_tree_model->get_iter(tree_path)->get_value(m_columns->id);

	// set the cursor

	auto begin = m_handler->get_begin_cursor();
	m_handler->set_tab_cursor(begin + id);

	m_handler->redraw();
}

bool Tree_active::on_kb_press(GdkEventKey* evt)
{
	if (evt->state == 0)
	{
		Window* wnd = m_handler->get_window();
		auto tab_it = wnd->get_current_tab();

		switch (evt->keyval)
		{
			case GDK_KEY_Left:
				wnd->set_pwd(tab_it->get_pwd().parent_path());
				return true;

			case GDK_KEY_Right:
			case GDK_KEY_Return:
				wnd->set_pwd(m_handler->get_cursor()->path);
				return true;
		}
	}

	return false;
}
