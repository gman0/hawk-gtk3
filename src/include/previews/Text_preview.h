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

#ifndef TEXT_PREVIEW_H
#define TEXT_PREVIEW_H

#include <memory>
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>
#include <hawk/Handler.h>

class Window;

class Text_preview : public hawk::Handler
{
private:
	Glib::RefPtr<Gtk::TextBuffer>  m_text_buffer;
	std::shared_ptr<Gtk::TextView> m_text_view;
	std::shared_ptr<Gtk::ScrolledWindow> m_scrolled_window;

public:
	Text_preview(const boost::filesystem::path& p,
		hawk::Column* parent_column,
		Window* win);
};

#endif // TEXT_PREVIEW_H
