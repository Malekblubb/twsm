//
// Copyright (c) 2014 Christoph Malek
// See LICENSE for more information.
//

#ifndef TWSM_ECON_HPP
#define TWSM_ECON_HPP


#include "popup_manager.hpp"
#include "ui_main_window.h"

#include <twl/network/network.hpp>
#include <mlk/types/types.h>

#include <QObject>


namespace Ui
{class main_window;}

namespace twsm
{
	class econ : public QObject
	{
		Q_OBJECT

		Ui::main_window& m_ui;
		popup_manager& m_popupmgr;

		std::map<std::size_t, twl::econ_client> m_clients;
		mlk::uint m_current_id{0};

	public:
		econ(Ui::main_window& ui, popup_manager& pm) :
			m_ui(ui),
			m_popupmgr(pm)
		{ }

		void update()
		{
			for(auto& a : m_clients)
				a.second.update();
		}

		void init()
		{
			// static objects
			QObject::connect(m_ui.m_cb_ec_servers, SIGNAL(currentIndexChanged(int)), this, SLOT(switch_server(int)));
			QObject::connect(m_ui.m_pb_ec_connect, SIGNAL(clicked()), this, SLOT(connect()));

			// main page
			QObject::connect(m_ui.m_pb_ec_change_map, SIGNAL(clicked()), this, SLOT(send_map_change()));
			QObject::connect(m_ui.m_pb_ec_reload_map, SIGNAL(clicked()), this, SLOT(send_reload_map()));
			QObject::connect(m_ui.m_pb_ec_start_recording, SIGNAL(clicked()), this, SLOT(send_start_recording()));
			QObject::connect(m_ui.m_pb_ec_stop_recording, SIGNAL(clicked()), this, SLOT(send_stop_recording()));
			QObject::connect(m_ui.m_pb_ec_set_broadcast, SIGNAL(clicked()), this, SLOT(send_broadcast()));
			QObject::connect(m_ui.m_pb_ec_restart_round, SIGNAL(clicked()), this, SLOT(send_restart()));
			QObject::connect(m_ui.m_pb_ec_send_chatmsg, SIGNAL(clicked()), this, SLOT(send_chat()));
			QObject::connect(m_ui.m_pb_ec_send_custom_cmd, SIGNAL(clicked()), this, SLOT(send_custom_cmd()));
			QObject::connect(m_ui.m_pb_ec_shutdown, SIGNAL(clicked()), this, SLOT(send_shutdown()));

			// players page
			QObject::connect(m_ui.m_pb_ec_refresh_palyers, SIGNAL(clicked()), this, SLOT(request_playerinfo()));
			QObject::connect(m_ui.m_pb_ec_kick_player, SIGNAL(clicked()), this, SLOT(kick_player()));
			QObject::connect(m_ui.m_pb_ec_ban_player, SIGNAL(clicked()), this, SLOT(ban_player()));
			QObject::connect(m_ui.m_pb_ec_permban_name, SIGNAL(clicked()), this, SLOT(permban_player()));

			// ban page
			QObject::connect(m_ui.m_pb_ec_ban_ip, SIGNAL(clicked()), this, SLOT(ban_ip()));
			QObject::connect(m_ui.m_pb_ec_unban_ip, SIGNAL(clicked()), this, SLOT(unban_ip()));
		}

		void connect(const mlk::ntw::ip_address& addr, const std::string& passwd)
		{
			auto iter(mlk::cnt::find_in_if([&addr](const decltype(m_clients)::value_type& c){return c.second.address() == addr;}, m_clients));

			// connected, client needs login
			if(iter != std::end(m_clients))
			{
				if(!iter->second.connected())
					iter->second.connect(addr);

				if(!iter->second.logged_in())
					iter->second.login(passwd);
			}

			// not connected, client needs connection and login
			else
				this->create_new_client(addr, passwd);
		}

		void send_command(const twl::econ_command& cmd)
		{
			auto index(m_ui.m_cb_ec_servers->currentIndex());
			if(index == 0)
			{
				// apply on all connected servers
				for(auto& a : m_clients)
					a.second.send_command(cmd);
			}
			else if(index > 0)
			{
				// get id (index != id)
				auto id(m_ui.m_cb_ec_servers->itemData(index).toInt());
				m_clients[id].send_command(cmd);
			}
		}

	private:
		void create_new_client(const mlk::ntw::ip_address& addr, const std::string& passwd)
		{
			// create new client
			m_clients.emplace(m_current_id, addr);
			auto& cl(m_clients[m_current_id]);

			// setup the events
			cl.on_log_added = [this, id = m_current_id](const std::string& str){this->_on_log_added(id, str);};
			cl.on_connection = [this, id = m_current_id]{this->_on_connection(id);};
			cl.on_login = [this, id = m_current_id]{this->_on_login(id);};
			cl.on_connection_timeout = [this, id = m_current_id]{this->_on_connection_timeout(id);};
			cl.on_connection_lost = [this, id = m_current_id]{this->_on_connection_lost(id);};
			cl.on_playerinfo = [this, id = m_current_id](const twl::econ_player_infos& i){this->_on_playerinfo(id, i);};

			// login
			cl.login(passwd);
			++m_current_id;
		}

		// on client events
		void _on_log_added(mlk::uint id, const std::string& str)
		{
			// check if we need to log (is the 'id' the current active server?)
			if(!this->is_current_active(id))
				return;

			m_ui.m_te_ec_log->moveCursor(QTextCursor::End);
			m_ui.m_te_ec_log->insertPlainText(str.c_str());
			m_ui.m_te_ec_log->moveCursor(QTextCursor::End);
		}

		void _on_connection(mlk::uint id)
		{
			m_popupmgr.create_popup<popup_type::info>("Econ connected to " + this->addr_from_id(id));

			for(auto i(1); i < m_ui.m_cb_ec_servers->count(); ++i)
				if(m_ui.m_cb_ec_servers->itemData(i).toInt() == static_cast<int>(id))
				{
					// we were already connected, choose right item
					m_ui.m_cb_ec_servers->setCurrentIndex(i);
					return;
				}

			// new connection
			m_ui.m_cb_ec_servers->addItem(m_clients[id].address().to_string().c_str(), id);
			m_ui.m_cb_ec_servers->setCurrentIndex(m_ui.m_cb_ec_servers->count() - 1);
		}

		void _on_login(mlk::uint id)
		{
			m_popupmgr.create_popup<popup_type::info>("Econ logged in on " + this->addr_from_id(id));
		}

		void _on_connection_timeout(mlk::uint id)
		{
			m_popupmgr.create_popup<popup_type::warning>("Connection attempt timed out for " + this->addr_from_id(id));
		}

		void _on_connection_lost(mlk::uint id)
		{
			m_popupmgr.create_popup<popup_type::error>("Connection to " + this->addr_from_id(id) + " lost", 5000);
		}

		void _on_playerinfo(mlk::uint id, const twl::econ_player_infos&)
		{
			if(!this->is_current_active(id))
				return;
			this->load_playerinfos_to_table();
		}


		// utils
		bool is_current_active(mlk::uint id)
		{
			return ((m_ui.m_cb_ec_servers->currentIndex() != 0) &&
					(m_ui.m_cb_ec_servers->currentData().toInt() == static_cast<int>(id)));
		}

		std::string addr_from_id(mlk::uint id) const
		{return m_clients.at(id).address().to_string();}


	private slots:
		// ui events
		void switch_server(int index)
		{
			// load last playerinfos
			this->load_playerinfos_to_table();

			if(index <= 0)
				return;

			auto id(m_ui.m_cb_ec_servers->itemData(index).toInt());

			// switch server, reset ui
			m_ui.m_te_ec_log->setText(m_clients[id].log().c_str());
		}

		void load_playerinfos_to_table()
		{
			m_ui.m_tw_ec_players->clearContents();
			m_ui.m_tw_ec_players->setRowCount(0);

			if(m_ui.m_cb_ec_servers->currentIndex() == 0)
				return;

			// get needed infos
			auto id(m_ui.m_cb_ec_servers->currentData().toInt());
			const auto& infos(m_clients[id].last_playerinfos());

			// insert...
			for(const auto& a : infos)
			{
				auto row(m_ui.m_tw_ec_players->rowCount());
				m_ui.m_tw_ec_players->insertRow(row);

				auto* id(new QTableWidgetItem{a.id().c_str()});
				auto* name(new QTableWidgetItem{a.name().c_str()});
				auto* score(new QTableWidgetItem{a.score().c_str()});
				auto* addr(new QTableWidgetItem{a.addr().c_str()});
				m_ui.m_tw_ec_players->setItem(row, 0, id);
				m_ui.m_tw_ec_players->setItem(row, 1, name);
				m_ui.m_tw_ec_players->setItem(row, 2, score);
				m_ui.m_tw_ec_players->setItem(row, 3, addr);
			}
		}

		void connect()
		{this->connect({QString{m_ui.m_le_ec_host->text() + ":" + m_ui.m_sb_ec_port->text()}.toStdString()}, m_ui.m_le_ec_passwd->text().toStdString());}

		void send_map_change()
		{this->send_command("change_map " + m_ui.m_le_ec_mapname->text().toStdString());}

		void send_reload_map()
		{this->send_command("reload");}

		void send_start_recording()
		{this->send_command("record " + m_ui.m_le_ec_recording_filename->text().toStdString());}

		void send_stop_recording()
		{this->send_command("stoprecord");}

		void send_broadcast()
		{this->send_command("broadcast " + m_ui.m_le_ec_broadcast->text().toStdString());}

		void send_restart()
		{this->send_command("restart " + m_ui.m_le_ec_restart_time->text().toStdString());}

		void send_chat()
		{this->send_command("say " + m_ui.m_le_ec_chatmsg->text().toStdString());}

		void send_custom_cmd()
		{this->send_command(m_ui.m_le_ec_custom_cmd->text().toStdString());}

		void send_shutdown()
		{this->send_command("shutdown");}

		void request_playerinfo()
		{
			if(m_ui.m_cb_ec_servers->currentIndex() == 0)
				return;

			// clear old infos
			m_ui.m_tw_ec_players->clearContents();
			m_ui.m_tw_ec_players->setRowCount(0);

			auto id(m_ui.m_cb_ec_servers->currentData().toInt());
			m_clients[id].request_playerinfo();
		}

		void kick_player()
		{
			for(auto& a : m_ui.m_tw_ec_players->selectedItems())
			{
				if(a->column() != 0)
					continue;
				this->send_command("kick " + a->text().toStdString());
			}
		}

		void ban_player()
		{
			for(auto& a : m_ui.m_tw_ec_players->selectedItems())
			{
				if(a->column() != 0)
					continue;
				this->send_command(QString{"ban %1 %2 %3"}.arg(a->text()).arg(m_ui.m_sb_ec_bantime->text()).arg(m_ui.m_le_ec_banreason->text()).toStdString());
			}
		}

		void permban_player()
		{
			// TODO
		}

		void ban_ip()
		{
			// TODO
		}

		void unban_ip()
		{
			// TODO
		}
	};
}


#endif // TWSM_ECON_HPP

