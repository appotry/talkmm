/*
 * =====================================================================================
 * 
 *       Filename:  MainWindow.h
 * 
 *    Description:  程序的主窗口
 * 
 *        Version:  1.0
 *        Created:  2007年11月25日 12时56分29秒 CST
 *       Revision:  none
 *       Compiler:  gcc
 * 
 *         Author:  wind (xihe), xihels@gmail.com
 *        Company:  cyclone
 * 
 * =====================================================================================
 */


#ifndef _MAINWINDOW_HH
#define _MAINWINDOW_HH


#include <gtkmm.h>
#include <gtkmm/checkbutton.h>
#include <sigc++/connection.h>
#include <libglademm/xml.h>
#include <glib/gi18n.h>
#include <map>
#include "TrayIcon.h"
#include "MVC.h"
#include "./config/rwxml.h"

#define main_ui "./main_window.glade"
typedef Glib::RefPtr <Gnome::Glade::Xml> GlademmXML;


class Talkmm;
class BuddyView;
class MsgWindow;


class MainWindow : public Gtk::Window {
	public:
		MainWindow(Talkmm* f_parent);
		~MainWindow();
		//void on_conf_window_close(ConfWindow* dlg);
		bool on_key_press_event(GdkEventKey* ev);
		void hide_window();
		void show_window();
		MsgWindow* open_session(const std::string& from);
		void close_session(const std::string& from);
		/** finsh login*/
		void on_signon();
		void on_roster_presence(const std::string& jid);
		void on_receive_message(const std::string& from,const std::string& message);
		void on_send_message(const std::string& to,const std::string& message);
		void on_file_receive(const std::string& from,const std::string& file);
		void on_incoming_call(const std::string& from);
		void on_init();
		void on_quit();

		void check_button_rememberme_clicked();
		void check_button_keeppasswd_clicked();

		void on_loginWindow_cancel();
		void on_login_emit();
		void on_login(CLogin::Handler* f_handler,CLogin::View::Func f_call);
		void signal_on_login(CLogin::Handler* f_handler,CLogin::View::Func f_call);

		GlademmXML				main_xml;
		Gtk::Notebook*				main_notebook;

		enum{ LOGIN_INIT=0,LOGIN_LOADING,LOGIN_FINISH};

	protected:
		bool on_delete_event(GdkEventAny* event);
		void on_menu_about();
	private:
		/** 属性窗口退出处理*/
		void on_conf_window_quit();
		/** 双击画面事件*/
		

	private:
		DealConf dealconf;
		typedef std::map<std::string,MsgWindow*> Session;
		Talkmm*					m_parent;
		Gtk::Entry*				entry_account;
		Gtk::Entry*				entry_passwd;
		Gtk::Button*				button_ok;
		Gtk::CheckButton*			check_button_rememberMe;
		Gtk::CheckButton* 			check_button_keeppasswd;
		Gtk::Button* 				button_cancel;
		Gtk::ProgressBar* 			progressbar_login;
		TrayIcon				*tray_icon;
		BuddyView*				list_view;
		Session*				m_session;
		//ConfWindow*			confwindow;
		int					window_width;
		int					window_height;
		int					window_x;
		int					window_y;
};

#endif // _MAINWINDOW_HH 

