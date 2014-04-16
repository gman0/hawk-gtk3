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

#include "previews/Image_preview.h"
#include "previews/Image_preview_hash.h"
#include "Window.h"

using namespace hawk;

Image_preview::Image_preview(const boost::filesystem::path& path,
	Column* parent_column,
	Window* win)
	:
	Handler{path, parent_column, get_handler_hash<Image_preview>()},
	m_image{new Gtk::Image{path.string()}}
{
	win->get_tree_box().pack_start(*m_image);

	// Work in progress
	// TODO: add scaling support
}
