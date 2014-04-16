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

#include <fstream>
#include "previews/Text_preview.h"
#include "previews/Text_preview_hash.h"
#include "Window.h"

constexpr int buffer_size = 2048;
char read_buffer[buffer_size];

using namespace hawk;

Text_preview::Text_preview(const boost::filesystem::path& p,
	hawk::Column* parent_column,
	Window* win)
	:
	Handler{p, parent_column, get_handler_hash<Text_preview>()},
	m_text_buffer{Gtk::TextBuffer::create()},
	m_text_view{new Gtk::TextView{m_text_buffer}},
	m_scrolled_window{new Gtk::ScrolledWindow}
{
	std::ifstream ifs {p.c_str()};
	ifs.read(read_buffer, buffer_size);
	read_buffer[ifs.gcount() - 1] = '\0';

	m_text_view->set_editable(false);
	m_text_buffer->set_text(read_buffer);

	m_scrolled_window->add(*m_text_view);
	win->get_tree_box().pack_start(*m_scrolled_window);
}
