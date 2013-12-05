#include "Window.h"
#include "DirPreview.h"

using namespace boost::filesystem;

#include <iostream>
using namespace std;
Dir_preview::Dir_preview(const path& p, Window* win, unsigned ncols)
	:
	hawk::List_dir{p},
	m_window{win}
{
	auto& tvec = m_window->get_treevec();
	m_id = tvec.size();

	Tree::Tree_flags flags {};

	if (m_id == --ncols)
		flags = Tree::TF_ACTIVE;
	else if (m_id > ncols)
		flags = Tree::TF_EXPAND;

	tvec.push_back({this, win->get_tree_box(), flags});
}

Dir_preview::~Dir_preview()
{
	auto& tvec = m_window->get_treevec();
	auto it = tvec.begin() + m_id;
	tvec.erase(it);
}
