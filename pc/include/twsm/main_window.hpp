//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSM_MAIN_WINDOW_HPP
#define TWSM_MAIN_WINDOW_HPP


#include "econ.hpp"
#include "popup_manager.hpp"
#include "serverbrowser.hpp"
#include "ui_main_window.h"
#include "servers_file.hpp"

#include <QMainWindow>
#include <QTimer>
#include <QDebug>


namespace Ui
{class main_window;}

namespace twsm
{
	class main_window : public QMainWindow
	{
		Q_OBJECT

		Ui::main_window* m_ui{new Ui::main_window};

		servers_file m_serversfile{"./twsm_servers"};
		popup_manager m_popupmgr{*this};

		server_browser m_srvbrowser{*m_ui, m_popupmgr};
		econ m_econ{*m_ui, m_serversfile, m_popupmgr};

		QTimer m_updatetimer;

	public:
		explicit main_window(QWidget* parent = nullptr) :
			QMainWindow{parent}
		{this->init();}

		~main_window()
		{delete m_ui;}

		Ui::main_window& ui() noexcept
		{return *m_ui;}

		popup_manager& popupmgr() noexcept
		{return m_popupmgr;}

		server_browser& srvbrowser() noexcept
		{return m_srvbrowser;}

		econ& econsole() noexcept
		{return m_econ;}

	private:
		void init()
		{
			// setup ui
			m_ui->setupUi(this);

			// setup components
			m_srvbrowser.init();
			m_econ.init();

			// setup timer
			m_updatetimer.start(0);
			this->connect(&m_updatetimer, SIGNAL(timeout()), this, SLOT(update()));
		}

	private slots:
		void update()
		{
			m_popupmgr.update();
			m_srvbrowser.update();
			m_econ.update();
		}

		// menu
		void on_m_tw_menu_itemSelectionChanged()
		{
			m_ui->m_sw_main->setCurrentIndex(m_ui->m_tw_menu->currentIndex().row() / 2);
		}
	};
}

#endif // TWSM_MAIN_WINDOW_HPP
