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

#ifndef TREE_ACTIVE_H
#define TREE_ACTIVE_H

#include "Tree.h"

class Dir_preview;
class Tree_active : public Tree
{
private:
	// With all the moving/copying around, we need to store the result
	// of sigc's connect() function in order to properly disconnect it
	// during Tree_active's destruction and then reconnect it again after copying.
	sigc::connection m_sig_cursor_change;
	sigc::connection m_sig_kb_press;
	sigc::connection m_sig_kb_release;

public:
	virtual ~Tree_active();
	Tree_active(const Tree_active& t);
	Tree_active(Tree_active&& t);
	Tree_active& operator=(const Tree_active& t);
	Tree_active& operator=(Tree_active&& t);

	Tree_active(Dir_preview* dp, Gtk::Box& box);

	virtual void update();

private:
	void register_signals();

	void on_cursor_changed();
	bool on_kb_press(GdkEventKey* event);
};

#endif // TREE_ACTIVE_H
