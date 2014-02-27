#include <cwctype>
#include <cctype>
#include <string>
#include <algorithm>
#include "previews/Tree.h"
#include "previews/Dir_preview.h"

using namespace std;

using sort_cmp_function =
	int(*)(const Gtk::TreeModel::iterator&, const Gtk::TreeModel::iterator&);

static int sort_name(const Gtk::TreeModel::iterator& a,
	const Gtk::TreeModel::iterator& b)
{
	string str_a;
	a->get_value(0, str_a);
	transform(str_a.begin(), str_a.end(), str_a.begin(), std::towlower);

	string str_b;
	b->get_value(0, str_b);
	transform(str_b.begin(), str_b.end(), str_b.begin(), std::towlower);

	// We have to convert the "special" unicode character to something
	// of a lower value so that alphabetically it comes prior to the entries
	// without this character.

	if (str_a[0] == '\xc2' && str_a[1] == '\xad')
	{
		str_a[0] = '\x1';
		str_a[1] = '\x1';
	}

	if (str_b[0] == '\xc2' && str_b[1] == '\xad')
	{
		str_b[0] = '\x1';
		str_b[1] = '\x1';
	}

	if (str_a > str_b)
		return 1;
	else if (str_a < str_b)
		return -1;
	else
		return 0;
}

Tree::Tree(Dir_preview* dp, Gtk::Box& box)
	:
	m_handler{dp},
	m_columns{new Model_columns},
	m_tree_model{Gtk::ListStore::create(*m_columns)},
	m_tree_view{new Gtk::TreeView},
	m_separator{new Gtk::VSeparator},
	m_scrolled_window{new Gtk::ScrolledWindow},
	m_empty{new Gtk::Label{"empty", Gtk::ALIGN_CENTER, Gtk::ALIGN_CENTER}}
{
	m_empty->set_justify(Gtk::JUSTIFY_CENTER);
	// setup the tree view

	m_tree_view->set_model(m_tree_model);
	m_tree_view->set_rules_hint();
	m_tree_view->append_column("Name", m_columns->entry);
	m_tree_view->set_headers_visible(false);

	// setup scrolling
	m_scrolled_window->add(*m_tree_view);
	m_scrolled_window->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	// TODO: this
	m_empty->set_no_show_all();
	box.pack_start(*m_empty, Gtk::PACK_SHRINK);

	m_tree_model->set_sort_func(m_columns->entry, sigc::ptr_fun(&sort_name));
	update();
}

void Tree::update()
{
	// insert the dir entries into the tree model,
	// but only if we have anything to insert

	if (!m_tree_model->children().empty())
		m_tree_model->clear();

	const hawk::List_dir::Dir_vector& vec = m_handler->get_contents();

	if (vec.empty())
	{
		m_tree_view->hide();
		m_scrolled_window->hide();
		m_empty->show();
		return;
	}

	m_tree_model->set_sort_column(0, Gtk::SORT_ASCENDING);

	m_empty->hide();

	// fill the TreeModel
	Gtk::TreeModel::Row row;
	size_t id = 0;
	Glib::ustring name;
	for (const auto& i : vec)
	{
		name.clear();

		row = *(m_tree_model->append());

		// mark directories with this unicode character (for sorting)
		if (i.status.type() == boost::filesystem::directory_file)
			name = "\xc2\xad";

		name.append(i.path.filename().c_str());

		row[m_columns->entry] = name;
		row[m_columns->id] = id++;
	}

	// setup the cursor

	Gtk::TreeModel::iterator tree_cursor;
	boost::system::error_code ec;

	if (m_handler->implicit_cursor())
	{
		tree_cursor = m_tree_model->children().begin();
		auto it = m_handler->get_contents().begin()
			+ tree_cursor->get_value(m_columns->id);
		m_handler->set_cursor(it);
	}
	else
	{
		tree_cursor = get_tree_iter(
			distance(vec.begin(), m_handler->get_const_cursor()),
			m_tree_model->children());
	}

	// set the TreeView cursor
	m_tree_view->set_cursor(Gtk::TreePath {tree_cursor});
}

Gtk::TreeModel::iterator Tree::get_tree_iter(int id,
		const Gtk::TreeModel::Children& children)
{
	auto it = children.begin();
	while (it->get_value(m_columns->id) != id)
		++it;

	return it;
}
