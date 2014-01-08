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

public:
	virtual ~Tree_active();
	Tree_active(const Tree_active& t);
	Tree_active(Tree_active&& t);
	Tree_active& operator=(const Tree_active& t);
	Tree_active& operator=(Tree_active&& t);

	Tree_active(Dir_preview* dp, Gtk::Box& box);

	virtual void update();

private:
	void on_cursor_changed();
};

#endif // TREE_ACTIVE_H
