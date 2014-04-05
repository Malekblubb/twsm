//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSM_SERVER_BROWSER
#define TWSM_SERVER_BROWSER


#include "ui_main_window.h"

#include <mlk/log/log.h>
#include <mlk/time/simple_timer.h>

#include <twl/network/network.hpp>

#include <QObject>
#include <QTableWidgetItem>


namespace Ui
{class main_window;}

namespace twsm
{
	class server_browser : public QObject
	{
		Q_OBJECT

		Ui::main_window& m_ui;

		twl::master_server m_masters;
		twl::game_server m_servers;

		mlk::tm::simple_timer m_stopwatch{0};

		bool m_refreshing_masters{false}, m_refreshing{false};

	public:
		server_browser(Ui::main_window& ui) :
			m_ui{ui}
		{ }

		void update()
		{
			m_masters.update();
			m_servers.update();
		}

		void init()
		{
			mlk::lout("server_browser", true) << "init. settings default masters. setting up events.";

			// setup default masters
			m_masters.add_master({"master1.teeworlds.com:8300"});
			m_masters.add_master({"master2.teeworlds.com:8300"});
			m_masters.add_master({"master3.teeworlds.com:8300"});
			m_masters.add_master({"master4.teeworlds.com:8300"});

			// setup events
			m_masters.on_finish = [this]{this->masters_finished();};
			m_servers.on_finish = [this]{this->servers_finished();};

			// ui events
			QObject::connect(m_ui.m_pb_srvb_refresh, SIGNAL(clicked()), this, SLOT(refresh()));
		}

	public slots:
		void refresh()
		{
			// reset ui
			m_ui.m_tw_srvb_list->clearContents();
			m_ui.m_tw_srvb_list->setRowCount(0);
			m_ui.m_lb_srvb_status->setText("Refreshing masters...");

			// request masterlist
			m_refreshing_masters = true;
			m_masters.request_list();
		}

	private:
		void masters_finished()
		{
			// master refresh finished

			m_refreshing_masters = false;
			m_refreshing = true;

			auto ips(m_masters.get_list());
			mlk::lout("server_browser") << "refreshed masters. got " << ips.size() << " ips.";
			m_ui.m_lb_srvb_status->setText(QString{"Refreshed masters, processing %1 servers..."}.arg(ips.size()));
			if(ips.empty())
				return;

			// make ready for server refreshing
			m_servers.reset();
			m_servers.add_masterlist(ips);
			m_servers.request_info();
			m_stopwatch.restart();
		}

		void servers_finished()
		{
			// server refresh finished

			m_refreshing = false;
			auto tm(m_stopwatch.elapsed_time());
			mlk::lout("server_browser") << "refreshed servers. (twl) processed " << m_servers.get_infos().size() << " servers in " << tm << "ms.";

			m_ui.m_tw_srvb_list->setSortingEnabled(false);
			for(auto& a : m_servers.get_infos())
			{
				m_ui.m_tw_srvb_list->insertRow(m_ui.m_tw_srvb_list->rowCount());
				auto* name(new QTableWidgetItem{a.name().c_str()});
				auto* type(new QTableWidgetItem{a.gametype().c_str()});
				auto* map(new QTableWidgetItem{a.mapname().c_str()});
				auto* players(new QTableWidgetItem{QString{"%1/%2"}.arg(a.numclients()).arg(a.maxclients())});
				auto* ping(new QTableWidgetItem{QString{"%1"}.arg(a.ping())});

				m_ui.m_tw_srvb_list->setItem(m_ui.m_tw_srvb_list->rowCount() - 1, 0, name);
				m_ui.m_tw_srvb_list->setItem(m_ui.m_tw_srvb_list->rowCount() - 1, 1, type);
				m_ui.m_tw_srvb_list->setItem(m_ui.m_tw_srvb_list->rowCount() - 1, 2, map);
				m_ui.m_tw_srvb_list->setItem(m_ui.m_tw_srvb_list->rowCount() - 1, 3, players);
				m_ui.m_tw_srvb_list->setItem(m_ui.m_tw_srvb_list->rowCount() - 1, 4, ping);
			}

			m_ui.m_tw_srvb_list->setSortingEnabled(true);
			m_ui.m_lb_srvb_status->setText("Servers refreshed.");
			mlk::lout("server_browser") << "ui took " << m_stopwatch.elapsed_time() - tm << " ms.";
		}
	};
}


#endif // TWSM_SERVER_BROWSER

