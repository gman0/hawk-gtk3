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
