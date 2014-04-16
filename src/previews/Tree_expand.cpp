/*
	Copyright (C) 2013-2014 Róbert "gman" Vašek <gman@codefreax.org>

	This file is part of hawk-gtk3.

	hawk-gtk3 is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 2 of the License, or
	(at your option) any later version.

	hawk-gtk3 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with hawk-gtk3.  If not, see <http://www.gnu.org/licenses/>.
*/

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

	// ellipsize long file names

	Gtk::CellRendererText* renderer =
		dynamic_cast<Gtk::CellRendererText*>(
			m_tree_view->get_column_cell_renderer(0));

	if (renderer)
		renderer->property_ellipsize() = Pango::ELLIPSIZE_MIDDLE;
}
