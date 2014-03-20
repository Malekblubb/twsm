//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSM_MAIN_WINDOW
#define TWSM_MAIN_WINDOW


#include "serverbrowser.hpp"
#include "ui_main_window.h"

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

		server_browser m_srvbrowser{*m_ui};

		QTimer m_updatetimer;

	public:
		explicit main_window(QWidget* parent = nullptr) :
			QMainWindow{parent}
		{this->init();}

		~main_window()
		{delete m_ui;}

	private:
		void init()
		{
			// setup ui
			m_ui->setupUi(this);

			// setup timer
			m_updatetimer.start(0);
			this->connect(&m_updatetimer, SIGNAL(timeout()), this, SLOT(update()));
		}

	private slots:
		void update()
		{
			m_srvbrowser.update();
		}

		void on_m_tw_menu_itemSelectionChanged()
		{
			m_ui->m_sw_main->setCurrentIndex(m_ui->m_tw_menu->currentIndex().row() / 2);
		}

		void on_m_pb_srvb_refresh_clicked()
		{
			m_srvbrowser.refresh();
		}
	};
}

#endif // TWSM_MAIN_WINDOW
