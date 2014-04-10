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
