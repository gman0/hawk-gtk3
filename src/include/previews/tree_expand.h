#ifndef TREE_EXPAND_H
#define TREE_EXPAND_H

#include "Tree.h"

class Dir_preview;
class Tree_expand : public Tree
{
public:
	Tree_expand(Dir_preview* dp, Gtk::Box& box);
};

#endif // TREE_EXPAND_H
