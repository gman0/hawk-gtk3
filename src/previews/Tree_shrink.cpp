#include "previews/Tree_shrink.h"

Tree_shrink::Tree_shrink(Dir_preview* dp, Gtk::Box& box)
	: Tree{dp, box}
{
	m_tree_view->set_sensitive(false);
	box.pack_start(get_widget(), Gtk::PACK_SHRINK);
	m_scrolled_window->set_size_request(90);

	m_separator->set_margin_left(2);
	m_separator->set_margin_right(2);
	box.pack_start(*m_separator, Gtk::PACK_SHRINK);

	// ellipsize long file names

	Gtk::CellRendererText* renderer =
		dynamic_cast<Gtk::CellRendererText*>(
			m_tree_view->get_column_cell_renderer(0));

	if (renderer)
		renderer->property_ellipsize() = Pango::ELLIPSIZE_MIDDLE;
}
