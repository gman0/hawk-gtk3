#ifndef TREE_SHRINK_H
#define TREE_SHRINK_H

#include "Tree.h"

class Dir_preview;
class Tree_shrink : public Tree
{
public:
	Tree_shrink(Dir_preview* dp, Gtk::Box& box);
};

#endif // TREE_SHRINK_H
