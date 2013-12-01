#include "DirPreview.h"

using namespace boost::filesystem;

#include <iostream>
using namespace std;
Dir_preview::Dir_preview(Gtk::Box& box, std::vector<Tree>* tvec, const path& p)
	:
	hawk::List_dir{p},
	m_tree_vec{tvec}
{
	m_id = tvec->size();
	tvec->push_back({this, box});
	// cout << p << endl;
}

Dir_preview::~Dir_preview()
{
	auto it = m_tree_vec->begin() + m_id;
	m_tree_vec->erase(it);
}
