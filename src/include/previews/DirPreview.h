#ifndef DIR_PREVIEW_H
#define DIR_PREVIEW_H

#include <hawk/handlers/dir.h>
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
