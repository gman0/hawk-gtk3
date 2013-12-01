#ifndef DIR_PREVIEW_H
#define DIR_PREVIEW_H

#include <hawk/handlers/dir.h>
#include "Tree.h"

class Dir_preview : public hawk::List_dir
{
private:
	std::vector<Tree>* m_tree_vec;
	size_t m_id;

public:
	Dir_preview(Gtk::Box& box, std::vector<Tree>* tvec,
		const boost::filesystem::path& p);
	~Dir_preview();
};

#endif // DIR_PREVIEW_H
