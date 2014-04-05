#include <iostream>
#include "Window.h"
#include "previews/Dir_preview.h"
#include "previews/Tree_active.h"
#include "previews/Tree_expand.h"
#include "previews/Tree_shrink.h"

using namespace boost::filesystem;

Dir_preview::Dir_preview(const path& p,
	hawk::Column* parent_column,
	Window* win,
	unsigned ncols)
	:
	hawk::List_dir{p, parent_column},
	m_window{win}
{
	auto& tvec = m_window->get_treevec();
	m_id = tvec.size();

	Tree* t;

	if (m_id < --ncols)
		t = new Tree_shrink {this, win->get_tree_box()};
	else if (m_id == ncols)
		t = new Tree_active {this, win->get_tree_box()};
	else
	{
		tvec[m_id] = new Tree_expand {this, win->get_tree_box()};
		return;
	}

	tvec.push_back(t);
}

Dir_preview::~Dir_preview()
{
	auto& tvec = m_window->get_treevec();

	delete tvec[m_id];

	// The pointer to the tree is zeroed-out rather
	// than erased from the vector so we won't get
	// mismatched IDs (which are "calculated" from
	// vector's size).
	tvec[m_id] = nullptr;
}

void Dir_preview::set_tab_cursor(const hawk::List_dir::Dir_cursor& cursor)
{
	boost::system::error_code ec;
	m_window->get_current_tab()->set_cursor(cursor, ec);

	if (ec)
		m_window->set_infobar_msg(ec.message());
}

hawk::List_dir::Dir_cursor Dir_preview::get_begin_cursor() const
{
	return m_window->get_current_tab()->get_begin_cursor();
}

void Dir_preview::redraw() const
{
	m_window->redraw();
}
