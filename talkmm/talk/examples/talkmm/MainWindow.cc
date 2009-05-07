/*
 * =====================================================================================
 *
 *       Filename:  MainWindow.cpp
 *
 *    Description:  程序的主窗口
 *
 *        Version:  1.0
 *        Created:  2007年11月25日 13时00分30秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wind (xihe), xihels@gmail.com
 *        Company:  cyclone
 *
 k* =====================================================================================
 */
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "talkmm.h"
#include "MainWindow.h"
//#include "ConfWindow.h"
#include <glib/gi18n.h>
#include <stdio.h>
#include <string.h>
#include "./config/rwxml.h"
#include "BuddyView.h"
#include "MsgWindow.h"

using namespace std;

enum{ LOGIN_INIT=0,LOGIN_LOADING,LOGIN_FINISH};

void MainWindow::show_window()
{
	this->show();
	this->move(window_x, window_y);
}

void MainWindow::hide_window()
{
	this->get_position(window_x, window_y);
	this->hide();
}

bool MainWindow::on_delete_event(GdkEventAny* event)
{
	this->get_position(window_x, window_y);
	return Gtk::Window::on_delete_event(event);
}

/*
void MainWindow::on_menu_view_preferences()
{
	if(NULL==confwindow)
	{
		confwindow = new ConfWindow(this);
		confwindow->signal_quit().connect(sigc::mem_fun(*this, &MainWindow::on_conf_window_quit));
	}
	else
		confwindow->raise();
}

*/
void MainWindow::on_menu_about()
{
	static Gtk::AboutDialog*  about(0);
	if(about == 0){
		std::vector<Glib::ustring> authors;
		authors.push_back("lerosua ");
		about = new Gtk::AboutDialog ;
		//about->set_logo(ui_logo);
		about->set_program_name("GMDemo");
		about->set_version("0.10");
		about->set_website("http://lerosua.org");
		about->set_copyright("Copyright (c) 2009 lerosua");
		about->set_comments(_("GMDemo is gtkmm program demo"));
		about->set_authors(authors);
		about->set_license (_("This program is licenced under GNU General Public Licence (GPL) version 2."));
		about->set_translator_credits("lerosua");


	}
	about->run();
	delete about;
	about=0;

}

void MainWindow::on_conf_window_quit()
{
}

/*
void MainWindow::on_conf_window_close(ConfWindow* dlg)
{
	g_assert(dlg == confwindow);
	delete confwindow;
	confwindow=NULL;
}
*/

MainWindow::MainWindow(Talkmm* f_parent):
	window_width(1)
	,window_height(1)
	//,confwindow(NULL)
	,tray_icon(NULL)
	,m_parent(f_parent)
	,m_session(new Session)
{
        main_xml = Gnome::Glade::Xml::create(main_ui, "main_notebook");
        main_notebook = dynamic_cast < Gtk::Notebook * > (main_xml->get_widget("main_notebook"));
        main_notebook->set_current_page(LOGIN_INIT);
        main_notebook->set_show_tabs(false);

	/** first page */

	/*
        Gtk::Button * button_cancel = dynamic_cast < Gtk::Button * > (main_xml->get_widget("login_cancel"));
        button_cancel->signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::on_quit));
	*/

	check_button_rememberMe = dynamic_cast <Gtk::CheckButton*>(main_xml->get_widget("rememberMe"));
	check_button_rememberMe->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::check_button_rememberme_clicked));
	check_button_keeppasswd = dynamic_cast <Gtk::CheckButton*>(main_xml->get_widget("keeppasswd"));
	check_button_keeppasswd->signal_clicked().connect(sigc::mem_fun(*this,&MainWindow::check_button_keeppasswd_clicked));

	button_ok = dynamic_cast <Gtk::Button*>(main_xml->get_widget("login_ok"));
        entry_account = dynamic_cast <Gtk::Entry*>(main_xml->get_widget("entry_account"));
        entry_passwd = dynamic_cast <Gtk::Entry*>(main_xml->get_widget("entry_passwd"));
	entry_passwd->signal_activate().connect(sigc::mem_fun(*this, &MainWindow::on_login_emit));

	if(dealconf.ReadXML() != -2){
		if(dealconf.username_r.size() != 0){
			check_button_rememberMe->set_active();
			entry_account->set_text(dealconf.username_r);
		}

		if(dealconf.password_r.size() != 0){
			check_button_keeppasswd->set_active();
			entry_passwd->set_text(dealconf.password_r);
		}
	}

	tray_icon = new TrayIcon(*this);

	/**third page*/
	Gtk::Container* list_window= dynamic_cast <Gtk::Container*>(main_xml->get_widget("listWindow"));
	list_view = Gtk::manage(new BuddyView(*this));
	list_window->add(*list_view);

	add(*main_notebook);
	//this->set_size_request(200,400);
	//this->set_size_request(256,641);
	this->set_size_request(240,576);
	this->show_all();
	this->resize(1,1);

}

MainWindow::~MainWindow()
{
	delete m_session;
}

bool MainWindow::on_key_press_event(GdkEventKey* ev)
{
	if(ev->type !=GDK_KEY_PRESS)
		return Gtk::Window::on_key_press_event(ev);
	
	switch(ev->keyval){
		case GDK_Escape:
			break;
		case GDK_F11:
			break;
		default:
			return Gtk::Window::on_key_press_event(ev);

	}
	return true;
}

void MainWindow::on_loginWindow_cancel()
{


}

void MainWindow::on_signon()
{
	main_notebook->set_current_page(LOGIN_FINISH);

}

void MainWindow::on_init()
{

	this->hide();

}

void MainWindow::check_button_rememberme_clicked()
{
	cout << "The rememberme was clicked: state=" << (check_button_rememberMe->get_active() ? "true" : "false") << endl;
	Glib::ustring username_t;
	Glib::ustring password_t;
	string password_t1;


	if(check_button_rememberMe->get_active() == true && check_button_keeppasswd->get_active() == true){
		username_t = entry_account->get_text();
		password_t = entry_passwd->get_text();
		dealconf.WriteXML(username_t, password_t);;
	}
	else if(check_button_rememberMe->get_active() == true && check_button_keeppasswd->get_active() == false){
		username_t = entry_account->get_text();
		password_t1.clear();
		dealconf.WriteXML(username_t, password_t1);;
	}
}

void MainWindow::check_button_keeppasswd_clicked()
{
	cout << "The keeppasswd was clicked: state=" << (check_button_keeppasswd->get_active() ? "true" : "false") << endl;

	Glib::ustring username_t;
        Glib::ustring password_t;
	string username_t1;


	if(check_button_keeppasswd->get_active() == true && check_button_rememberMe->get_active() == true){
		username_t = entry_account->get_text();
		password_t = entry_passwd->get_text();
		dealconf.WriteXML(username_t, password_t);;
	}
	else if(check_button_keeppasswd->get_active() == true && check_button_rememberMe->get_active() == false){
		password_t = entry_passwd->get_text();
		username_t1.clear();
		dealconf.WriteXML(username_t1, password_t);;
	}
}

void MainWindow::on_login_emit()
{
	button_ok->clicked();
}

void MainWindow::on_login(CLogin::Handler* f_handler,CLogin::View::Func f_call)
{
        Glib::ustring name = entry_account->get_text();
        Glib::ustring passwd = entry_passwd->get_text();

        if (name.empty() || passwd.empty())
                return ;
        main_notebook->set_current_page(LOGIN_LOADING); //设置当前状态为登录中

        if (!(f_handler->*f_call)(name, passwd)) { // 登录失败
		printf("login false\n");
	}else{
		check_button_rememberme_clicked();
		check_button_keeppasswd_clicked();
	}
}


void MainWindow::signal_on_login(CLogin::Handler* f_handler,CLogin::View::Func f_call)
{
	button_ok->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this,&MainWindow::on_login),f_handler,f_call));
}

void MainWindow::on_quit()
{
}
void MainWindow::on_roster_presence(const std::string& jid)
{

	list_view->add(jid);
}

void MainWindow::on_receive_message(const std::string& from,const std::string& message)
{
#if 0
	MsgWindow* msg_window=NULL;
	Session::iterator iter = m_session->find(from);
	if(iter != m_session->end()){
		msg_window = iter->second;
	}
	else{
		msg_window =new MsgWindow(this,from);
		(*m_session)[from]=msg_window;
	}
#endif
	MsgWindow* msg_window = open_session(from);

	std::string utext = from +" : "+message+"\n";
	msg_window->show_message(utext);

}

MsgWindow* MainWindow::open_session(const std::string& from)
{
	MsgWindow* msg_window=NULL;
	Session::iterator iter = m_session->find(from);
	if(iter != m_session->end()){
		msg_window = iter->second;
	}
	else{
		msg_window =new MsgWindow(this,from);
		(*m_session)[from]=msg_window;
	}

	msg_window->show();
	return msg_window;
}

void MainWindow::on_send_message(const std::string& to,const std::string& message)
{
	m_parent->SendTexte(to,message);
}

void MainWindow::on_incoming_call(const std::string& from)
{

        Gtk::MessageDialog dialog(*this, _("Incoming Call"), false,
                                  Gtk::MESSAGE_QUESTION,
                                  Gtk::BUTTONS_OK_CANCEL);

        Glib::ustring msg = from +" Calling you! Are you accept? ";
        dialog.set_secondary_text(msg);
        int result = dialog.run();
        switch (result) {
        case (Gtk::RESPONSE_OK): {
					 m_parent->AnswerCall(true);
					 break;
				 }
        case (Gtk::RESPONSE_CANCEL): {
					 m_parent->AnswerCall(false);
                        break;
                }
        default: {
					 m_parent->AnswerCall(false);
                        break;
                }
	}

}

void MainWindow::on_file_receive(const std::string& from,const std::string& file)
{
        Gtk::MessageDialog dialog(*this, _("Receive File"), false,
                                  Gtk::MESSAGE_QUESTION,
                                  Gtk::BUTTONS_OK_CANCEL);

        Glib::ustring msg = from +" want to send a file to you "+"("+file+")";
        dialog.set_secondary_text(msg);
        int result = dialog.run();
        switch (result) {
        case (Gtk::RESPONSE_OK): {
					 m_parent->AnswerFile(true);
					 break;
				 }
        case (Gtk::RESPONSE_CANCEL): {
					 m_parent->AnswerFile(false);
                        break;
                }
        default: {
					 m_parent->AnswerFile(false);
                        break;
                }
	}
}
