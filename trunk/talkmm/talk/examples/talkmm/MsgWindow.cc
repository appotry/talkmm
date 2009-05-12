/*
 * =====================================================================================
 *
 *       Filename:  MsgWindow.cc
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2009年04月29日 00时28分33秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  lerosua (), lerosua@gmail.com
 *        Company:  Cyclone
 *
 * =====================================================================================
 */

#include <gtkmm/textbuffer.h>
#include "MainWindow.h"
#include "MsgWindow.h"

MsgWindow::MsgWindow(MainWindow* f_parent,
		     const std::string& f_jid):m_parent(f_parent),
		     m_jid(f_jid)
{
        msg_xml = Gnome::Glade::Xml::create(msg_ui, "vbox_main");
	Gtk::VBox* vbox_main= dynamic_cast < Gtk::VBox * > (msg_xml->get_widget("vbox_main"));
	entry_send = dynamic_cast<Gtk::Entry*> (msg_xml->get_widget("entry_send"));
	textview_msg = dynamic_cast<Gtk::TextView*> (msg_xml->get_widget("textview_msg"));

	entry_send->signal_activate().connect(sigc::mem_fun(*this, &MsgWindow::send_message));
	Gtk::Button* button_send_file = dynamic_cast<Gtk::Button*>(msg_xml->get_widget("button_send_file"));
	button_call = dynamic_cast<Gtk::Button*>(msg_xml->get_widget("button_call"));

	button_send_file->signal_clicked().connect(sigc::mem_fun(*this, &MsgWindow::on_send_file));
	button_call->signal_clicked().connect(sigc::mem_fun(*this, &MsgWindow::on_button_call));

	//vbox_file = dynamic_cast<Gtk::VBox*>(msg_xml->get_widget("vbox_file"));
	progress_frame = dynamic_cast<Gtk::Frame*>(msg_xml->get_widget("progress_frame"));
	//progressbar_send_file = dynamic_cast<Gtk::Progressbar*>(msg_xml->get_widget("progressbar_send_file"));
	//progressbar_send_file->signal_clicked().connect(sigc::mem_fun(*this, &MsgWindow::on_send_file));

	add(*vbox_main);
	//this->set_size_request(400,300);
	this->set_size_request(350,270);
	this->set_title(m_jid);
	this->show_all();
	//vbox_file->hide();
	progress_frame->hide();
}
MsgWindow::~MsgWindow()
{

}

bool MsgWindow::on_delete_event(GdkEventAny* event)
{
	m_parent->close_session(m_jid);
	delete this;

}
void MsgWindow::show_message(const std::string& msg)
{
	textview_msg->set_wrap_mode(Gtk::WRAP_WORD);
        Glib::RefPtr < Gtk::TextBuffer > buffer = textview_msg->get_buffer();
        Gtk::TextBuffer::iterator end = buffer->end();
        Gdk::Rectangle rect;
        textview_msg->get_visible_rect(rect);

        int y = -1;
        int height = -1;
        textview_msg->get_line_yrange(end, y, height);

        buffer->place_cursor(buffer->insert(end, msg));

        if (y < rect.get_y() + rect.get_height() + 16) // 最后一行可见，也就是用户没有向上滚动
                textview_msg->scroll_to_mark(buffer->get_insert(), 0); // 插入文本后也要向下滚动，使最后一行继续可见

}

void MsgWindow::send_message()
{
	std::string text = entry_send->get_text();
	m_parent->on_send_message(m_jid,text);
	std::string utext="me : "+ text ;
	show_message(utext);
	show_message("\n");
	entry_send->set_text("");

}

void MsgWindow::on_send_file()
{
        Gtk::FileChooserDialog dialog(_("Please select a file"), Gtk::FILE_CHOOSER_ACTION_OPEN);
        dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
        dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

        Gtk::FileFilter filter_png;

        filter_png.set_name("PNG files");
        filter_png.add_mime_type("image/png");
        dialog.add_filter(filter_png);

        Gtk::FileFilter filter_jpg;
        filter_jpg.set_name("JPG files");
        filter_jpg.add_mime_type("image/jpg");

        dialog.add_filter(filter_jpg);

        Gtk::FileFilter filter_gif;
        filter_gif.set_name("GIF files");
        filter_gif.add_mime_type("image/gif");

        dialog.add_filter(filter_gif);

        Gtk::FileFilter filter_any;
        filter_any.set_name("Any files");
        filter_any.add_mime_type("*");

        dialog.add_filter(filter_any);

        std::string filename ;

        //dialog.set_current_folder("~/Desktop");
        int result = dialog.run();

        switch (result) {
        case (Gtk::RESPONSE_OK): {
                        filename = dialog.get_filename(); //注意：这里取回的并不是Glib::ustring, 而是std::string.
                        break;
                }

        case (Gtk::RESPONSE_CANCEL):
                        std::cout << "Cannel choose icon" << std::endl;
                        return ;
        default:
                        std::cout << "Cannel choose icon" << std::endl;
                        return ;
        }

	m_parent->on_send_file(m_jid,filename);
}

void MsgWindow::on_button_call()
{
	m_parent->send_call_to(m_jid);
	cout << "call someone" << endl;
}
void MsgWindow::file_tranfer_start()
{
	//vbox_file->show();
	progress_frame->show();
}

void MsgWindow::file_tranfer_end()
{
	//vbox_file->hide();
	progress_frame->hide();
}
