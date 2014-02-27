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
