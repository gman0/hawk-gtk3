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

#ifndef DIR_PREVIEW_H
#define DIR_PREVIEW_H

#include <hawk/handlers/List_dir.h>
#include <hawk/Tab.h>
#include "Tree.h"

class Window;
class Dir_preview : public hawk::List_dir
{
private:
	Window* m_window;
	size_t m_id;

public:
	Dir_preview(const boost::filesystem::path& p,
		hawk::Column* parent_column,
		Window* win,
		unsigned ncols);
	~Dir_preview();

	void set_tab_cursor(const hawk::List_dir::Dir_cursor& cursor);
	hawk::List_dir::Dir_cursor get_begin_cursor() const;
	Window* get_window() { return m_window; }
	void redraw() const;
};

#endif // DIR_PREVIEW_H
