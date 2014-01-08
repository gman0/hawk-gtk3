#ifndef IMAGE_PREVIEW_H
#define IMAGE_PREVIEW_H

#include <memory>
#include <gtkmm/image.h>
#include <hawk/Handler.h>

class Window;

class Image_preview : public hawk::Handler
{
private:
	std::shared_ptr<Gtk::Image> m_image;

public:
	Image_preview(const boost::filesystem::path& p,
		hawk::Column* parent_column,
		Window* win);
};

#endif // IMAGE_PREVIEW_H
