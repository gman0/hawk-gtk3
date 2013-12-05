#ifndef DIR_PREVIEW_H
#define DIR_PREVIEW_H

#include <hawk/handlers/dir.h>
#include "Tree.h"

class Window;
class Dir_preview : public hawk::List_dir
{
private:
	Window* m_window;
	size_t m_id;

public:
	Dir_preview(const boost::filesystem::path& p, Window* win,
		unsigned ncols);
	~Dir_preview();
};

#endif // DIR_PREVIEW_H
