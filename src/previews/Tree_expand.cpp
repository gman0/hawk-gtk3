#include "previews/Tree_expand.h"

Tree_expand::Tree_expand(Dir_preview* dp, Gtk::Box& box)
	: Tree{dp, box}
{
	m_tree_view->set_sensitive(false);

	box.pack_start(get_widget(),
			(m_tree_model->children().empty() ?
			 Gtk::PACK_SHRINK : Gtk::PACK_EXPAND_WIDGET));

	// Let's assume that the Tree_expand is the last Tree widget
	// so we don't need a separator for it.
}